import random

MAX_X = 20 # limit: 2000
MAX_Y = 50 # limit: 2000
MAX_Z = 3   # limit: 32
MIN_PIN = 2
MAX_PIN = 5  # # of pin per mc
MIN_BLK = 1
MAX_BLK = 3  # # of blk per mc
CELL_DENSITY = 3 # cell per grid
CELL_MOVE_RATIO = 0.2 # ratio of # movable cells
FIXED_RATIO = 0.2
MASTERCELL_RATIO = 10 # num of cells per mc
SAME_EXTRA_DEMAND = 2
ADJH_EXTRA_DEMAND = 1
SUPPLY_RATIO = 2
BLK_DEMAND = 1
TOTAL_EXTRA_DEMAND_RATIO = 4 # must be even
SET_OF_EXTRA_DEMAND = 2 # num of extra demand per set
TOTAL_CELL = MAX_X*MAX_Y*CELL_DENSITY
TOTAL_NET = TOTAL_CELL
PIN_PER_NET = 5 
MAX_CELL_MOVE = TOTAL_CELL*CELL_MOVE_RATIO
TOTAL_MASTERCELL = TOTAL_CELL/MASTERCELL_RATIO
SUPPLY = CELL_DENSITY*ADJH_EXTRA_DEMAND*SAME_EXTRA_DEMAND*SUPPLY_RATIO
TOTAL_EXTRA_DEMAND = TOTAL_MASTERCELL*TOTAL_EXTRA_DEMAND_RATIO


random.seed('hi')

def output(i):
    return str(int(i))

###### max cell move
print('MaxCellMove ' + output(MAX_CELL_MOVE))

###### grid boundary
print('GGridBoundaryIdx 1 1 ' + output(MAX_X) + " " + output(MAX_Y))

###### layers definition
print('NumLayer ' + output(MAX_Z))
for x in range(1, int(MAX_Z+1)):
    if x%2 :
        d = 'H'
    else:
        d = 'V'
    print('Lay M{x} {x} {d} {SUPPLY}'.format(x=x, d=d, SUPPLY=int(SUPPLY)))

###### MC definition
MC_dic = dict()
print('NumMasterCell ' + output(TOTAL_MASTERCELL))
for x in range(1, int(TOTAL_MASTERCELL+1)):
    p = random.randint(MIN_PIN, MAX_PIN)
    b = random.randint(MIN_BLK, MAX_BLK)
    MC_dic[str(x)] = p
    print('MasterCell MC{x} {p} {b}'.format(x=x, p=p, b=b))
    for y in range(1, p+1):
        m = random.randint(1, MAX_Z)
        print('Pin P{y} M{m}'.format(y=y, m=m))
    for y in range(1, b+1):
        m = random.randint(1, MAX_Z)
        d = random.randint(0, BLK_DEMAND)
        print("Blkg B{y} M{m} {d}".format(y=y, m=m, d=d))

###### extra demand definition
set_of_mc = set()
for x in range (0, int(TOTAL_EXTRA_DEMAND/2)):
    a = random.randint(1, int(TOTAL_MASTERCELL)-1)
    b = random.randint(int(a+1), int(TOTAL_MASTERCELL))
    while (a, b) in set_of_mc:
        a = random.randint(1, int(TOTAL_MASTERCELL)-1)
        b = random.randint(int(a+1), int(TOTAL_MASTERCELL))
    set_of_mc.add((a, b))
print('NumNeighborCellExtraDemand ' + output(TOTAL_EXTRA_DEMAND))
for pair in set_of_mc:
    print('sameGGrid MC{a} MC{b} M1 {d}'.format(a=pair[0], b=pair[1], d=SAME_EXTRA_DEMAND))
for pair in set_of_mc:
    print('adjHGGrid MC{a} MC{b} M1 {d}'.format(a=pair[0], b=pair[1], d=ADJH_EXTRA_DEMAND))

###### cell instance definition
print('NumCellInst ' + output(TOTAL_CELL))
for i in range(1, int(TOTAL_CELL+1)):
    x = random.randint(1, MAX_X)
    y = random.randint(1, MAX_Y)
    if i < TOTAL_CELL-MAX_CELL_MOVE:
        m = 'Movable'
    else:
        m = 'Fixed'
    if int(i/MASTERCELL_RATIO) >= TOTAL_MASTERCELL:
        mc = int(i/MASTERCELL_RATIO)
    else:
        mc = int(i/MASTERCELL_RATIO+1)
    print('CellInst C{i} MC{mc} {x} {y} {m}'.format(i=i, mc=mc, x=x, y=y, m=m))

###### netlist definition
print('NumNets ' + output(TOTAL_NET))
for i in range(1, int(TOTAL_NET+1)):
    n = random.randint(1, int(PIN_PER_NET))
    cell = set()
    while len(cell) != n:
        c = random.randint(1, int(TOTAL_CELL))
        cell.add(int(c))
    print('Net N{i} {n} NoCstr'.format(i=i, n=n))
    for val in cell:
        if int(val/MASTERCELL_RATIO) >= int(TOTAL_MASTERCELL):
            mc = int(val/MASTERCELL_RATIO)
        else:
            mc = int(val/MASTERCELL_RATIO+1)
        p = random.randint(1, int(MC_dic[str(mc)]))
        print('Pin C{c}/P{p}'.format(c=val, p=p))
