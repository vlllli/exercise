from functools import total_ordering
from math import pow

def cube(k):
    return pow(k,3)
def summation(n,term):
    total,k=0,1
    while k<=n:
        total,k=total+term(k),k+1
    return total

result=summation(3,cube)
print(result)
