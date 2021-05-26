
В этом файле:

    n — общее число островов, включая остров базирования,
    p — стоимость прохождения кораблём единичного расстояния,
    k и M — целочисленные параметры, ограничивающие количество денег, 
    могут содержаться на последовательно посещаемых пиратом островах,
    x_i​ и y_i​ — целочисленные координаты i-го острова и
    m_i​ — целочисленное количество денежных единиц, имеющихся на острове.
    Все параметры, кроме mi​, находятся в диапазоне [0, 1000]. 
    Параметры m_i​ не превосходят 100000.

Наша задача — предоставить Капитану последовательность номеров островов, 
которые ему следует посетить. Первый остров — это остров базирования,
так что последовательность начинается и заканчивается числом “1”.
Числа в последовательности разделены пробелами, и все числа (кроме “1”) различны.
"""

import math
import numpy as np
import scipy.optimize as op
from collections import namedtuple
Island = namedtuple('Point', 'x y m number')

def func(x):
    mem = islands[:]
    answer, profit = get_track(x[0], x[1], x[2], mem, x[3])
    return profit * -1

def get_optimal_eps(p, k, M, islands):
    bounds = [(p, p), (k, k), (M, M), (-1, 1)] #Область определения функции (Оптимизация в данной области)
    #ans.x - значение параметра x.
    #ans.nfev - количество вычислений функции.
    # Следующий метод не использует градиент
    ans = op.differential_evolution(func, bounds)
    print("\n differential_evolution, bounds = ", bounds, " —------— \n", ans)
    return 0


def correct_m_bound(islands, M):
    i = 0
    len_islands = len(islands)
    while i < len_islands:
        if (islands[i].m > M):
            islands.pop(i)
            i -= 1  
            len_islands -= 1
        i += 1
    return

def travel_cost(island1, island2, p):
    return  math.sqrt((island1.x - island2.x) ** 2 + (island1.y - island2.y) ** 2) * p

def improve_answer(answer, p, islands, eps): #если выгоднее плыть домой раньше - изменим ответ
    max_profit = islands[0].m
    current_profit = max_profit
    profit_if_end = -1
    endpoint = 0
    for i in range(1, len(answer) - 1): #первый и последний элементы в answer -- дом
        current_profit += answer[i].m - travel_cost(answer[i - 1], answer[i], p)        
        #print("in improve_answer: for:: current_profit:", current_profit, "current profit if end", current_profit - travel_cost(answer[i], islands[0], p), "Maxprofit:", max_profit, "travel cost", travel_cost(answer[i - 1], answer[i], p))
        #находим наибольший выигрыш (с учетом путешествия домой)
        profit_if_end = current_profit - travel_cost(answer[i], islands[0], p)
        if profit_if_end > max_profit: 
            max_profit = profit_if_end
            endpoint = i
    answer = answer[0 : endpoint + 1]
    answer.append(islands[0])
    print("PROFIT:", max_profit, "FULL TRACK PROFIT:", profit_if_end, "EPS:", eps)
    return answer, max_profit

def improve_optimum(optimal_island_index, from_island, to_island, islands, current_money, p, k, M):
    sucess, new_optimal_island_index = get_optimal_island2(from_island, to_island, islands, current_money, p, k, M)
    if sucess:
        return new_optimal_island_index
    else:
        return optimal_island_index

def get_optimal_island2(from_island, to_island, islands, current_money, p, k, M):
    optimum = -1
    optimal_island_index = -1
    for i in range(1, len(islands)): #перебираем все доступные острова кроме дома       
        #print("IMPROVE: for:: From island:", from_island, "Not checked:", islands[i:], "Money:", current_money, "Opt:", optimum)
        if islands[i].m + current_money <= M: #остров не должен слишком обогатить пирата
            current_optimum = improve_optimum_condition(from_island, to_island, islands[i], p, k, M) #близжайшая точка
            #print("in IMPROVE: if:: From island:", from_island, "Not checked:", islands[i:], "Money:", current_money, "Opt:", optimum, "Curr opt:", current_optimum)
            #Если оптимума нет или наша точка оптимальнее И не в убыток ли туда заезжать
            if (optimum == -1 or current_optimum < optimum) and improve_optimum_condition2(from_island, to_island, islands[i], p, k, M):
                optimum = current_optimum
                optimal_island_index = i               
    if optimum < 0: #не подошло ни одного острова
        #print("IMPROVE CAN'T GET OPTIMAL ISLAND")
        return False, optimal_island_index
    else:
        #print("in IMPROVE END:: From island:", from_island, "Not checked:", islands[i:], "Money:", current_money, "Opt:", optimum, "Opt island:", islands[optimal_island_index])
        return True, optimal_island_index
    
def improve_optimum_condition(from_island, to_island, current_island, p, k, M):
    return travel_cost(from_island, current_island, p)

def improve_optimum_condition2(from_island, to_island, current_island, p, k, M):
    #print("current_island", current_island,"improve_optimum_condition2", current_island.m - travel_cost(from_island, current_island, p) - travel_cost(current_island, to_island, p))
    if current_island.m < M / k and travel_cost(from_island, current_island, p) + travel_cost(current_island, to_island, p) - current_island.m <= travel_cost(from_island, to_island, p):
        return True
    else:
        return False


def change_get_track_parameters(islands_visited, current_money, island_index, array_of_islands, answer):
    islands_visited += 1
    if islands_visited < k:
        current_money += array_of_islands[island_index].m
    else:
        current_money += array_of_islands[island_index].m 
        current_money -= answer[-k].m #удаляем первый из цепочки длиной k
    island = array_of_islands[island_index]
    array_of_islands.pop(island_index)
    return island, islands_visited, current_money, array_of_islands
        
def optimal_condition(from_island, to_island, p, k, M, eps): #condition for minimize!
    #return abs(M / (k) - to_island.m) * eps + travel_cost(from_island, to_island, p)
    #return travel_cost(from_island, to_island, p)
    return abs(M / k - to_island.m) * eps + travel_cost(from_island, to_island, p)

def get_optimal_island(from_island, islands, current_money, p, k, M, eps):
    optimum = -1
    optimal_island_index = -1
    for i in range(1, len(islands)): #перебираем все доступные острова кроме дома       
        #print("in get_optimal_island: for:: From island:", from_island, "Not checked:", islands[i:], "Money:", current_money, "Opt:", optimum)
        if islands[i].m + current_money <= M: #остров не должен слишком обогатить пирата
            current_optimum = optimal_condition(from_island, islands[i], p, k, M, eps)
            #print("in get_optimal_island: if:: From island:", from_island, "Not checked:", islands[i:], "Money:", current_money, "Opt:", optimum, "Curr opt:", current_optimum)
            if optimum == -1 or current_optimum < optimum: #Если оптимума нет или наша точка оптимальнее
                optimum = current_optimum
                optimal_island_index = i               
    if optimum == -1: #не подошло ни одного острова
        #print("CAN'T GET OPTIMAL ISLAND")
        return False, optimal_island_index
    else:
        #print("in get_optimal_island END:: From island:", from_island, "Not checked:", islands[i:], "Money:", current_money, "Opt:", optimum, "Opt island:", islands[optimal_island_index])
        return True, optimal_island_index

def get_track(p, k, M, islands, eps):
    answer = [islands[0]]
    current_island = islands[0]
    islands_visited = 1
    current_money = islands[0].m
    profit = -1
    while True: #Повторяем, пока еще можно найти оптимальный остров (succes of get_optimal_island)
        #print("in while:: Current:", current_island, "Not visited:", islands, "Money:", current_money)        
        sucess, optimal_island_index = get_optimal_island(current_island, islands, current_money, p, k, M, eps)
        if sucess:
            #print("in sucess:: Current:", current_island, "Not visited:", islands, "Money:", current_money)
            #optimal_island_index = improve_optimum(optimal_island_index, current_island, islands[optimal_island_index], islands, current_money, p, k, M) 
            answer.append(islands[optimal_island_index])
            current_island, islands_visited, current_money, islands = change_get_track_parameters(islands_visited, current_money, optimal_island_index, islands, answer)
        else:
            answer.append(islands[0])
            #print("in else:: Current:", current_island, "Not visited:", islands, "Money:", current_money, "Answer", answer)
            answer, profit = improve_answer(answer, p, islands, eps)
            #print("answer:", answer)
            break
    return answer, profit

#file = open('yurii_task2.txt', 'r')    
file = open('dataset_328552_3.txt', 'r')
n, p, k, M = list(map(int, file.readline().split()))
#n, p, k, M = list(map(int, input().split()))
if k == 0:
    print("ERROR")

islands = []
print(n, p, k, M)
summ = 0
for i in range(n):
    #x, y, m = list(map(int, input().split()))
    x, y, m = list(map(int, file.readline().split()))
    summ += m
    islands.append(Island(x, y, m, i + 1))
   
correct_m_bound(islands, M) #Удаляем острова с m привышающим лимит (m > M)
if islands == []:
    print ("ERROR")
#print(islands)
#maxprofit = -1
#maxeps = 1
#for eps in np.arange(-0.5, 1, 0.01):
#    mem = islands[:]
#    track, profit = get_track(p, k, M, mem, eps)
#    if maxprofit < profit:
#        maxprofit = profit
#        maxeps = eps

#eps = get_optimal_eps(p, k, M, islands)   DIFFERENTIAL EVALUATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
eps = 0.04631185

track, profit = get_track(p, k, M, islands, eps)
for island in track:
    print(island.number, end = ' ')
file.close()
