import subprocess
import sys

USERS = int(sys.argv[1])
MAX = int(sys.argv[2])
INTERVAL = int(sys.argv[3])
FILENAME = sys.argv[4]
FILE = sys.argv[5]
SIMTIMES = int(sys.argv[6])


CELS = 7
COUNT = 1
time = 30
TOTALNAME = ""
PF_ = []
EXPPF_ = []
MLWDF_ = []


while USERS != MAX+INTERVAL:
    while COUNT != SIMTIMES+1:
        TOTALNAME = f'Simulation_{COUNT}_Multi_PF_{USERS}U{CELS}C.sim'
        k = subprocess.run(['./make_fairness_index.awk',
                            f'{TOTALNAME}'], capture_output=True)
        #print(k)
        PF_.append(float(k.stdout))
        COUNT += 1
    COUNT = 1

    while COUNT != SIMTIMES+1:
        TOTALNAME = f'Simulation_{COUNT}_Multi_EXPPF_{USERS}U{CELS}C.sim'
        k = subprocess.run(['./make_fairness_index.awk',
                            f'{TOTALNAME}'], capture_output=True)
        #print(k)
        EXPPF_.append(float(k.stdout))
        COUNT += 1
    COUNT = 1

    while COUNT != SIMTIMES+1:
        TOTALNAME = f'Simulation_{COUNT}_Multi_MLWDF_{USERS}U{CELS}C.sim'
        k = subprocess.run(['./make_fairness_index.awk',
                            f'{TOTALNAME}'], capture_output=True)
        #print(k)
        MLWDF_.append(float(k.stdout))
        COUNT += 1
    COUNT = 1



    USERS += INTERVAL

'''print(f'PF{PF_}')
print(f'EXPPF{EXPPF_}')
print(f'MLWDF{MLWDF_}')
'''


PF_avg = []
EXPPF_avg = []
MLWDF_avg = []


for i in range(int(len(PF_)/SIMTIMES)):
    tmp = sum(PF_[SIMTIMES*i:SIMTIMES*(i+1)-1])/SIMTIMES
    PF_avg.append(tmp)

    tmp = sum(EXPPF_[SIMTIMES*i:SIMTIMES*(i+1)-1])/SIMTIMES
    EXPPF_avg.append(tmp)

    tmp = sum(MLWDF_[SIMTIMES*i:SIMTIMES*(i+1)-1])/SIMTIMES
    MLWDF_avg.append(tmp)



'''print(PF_avg)
print(EXPPF_avg)
print(MLWDF_avg)
'''

c = 10
with open('./PF_Y1_Fairness.dat', 'w', encoding='UTF-8') as h:
    for i in PF_avg:
        h.writelines(f'{c} {i}\n')
        c += INTERVAL

c = 10
with open('./EXPPF_Y1_Fairness.dat', 'w', encoding='UTF-8') as h:
    for i in EXPPF_avg:
        h.writelines(f'{c} {i}\n')
        c += INTERVAL

c = 10
with open('./MLWDF_Y1_Fairness.dat', 'w', encoding='UTF-8') as h:
    for i in MLWDF_avg:
        h.writelines(f'{c} {i}\n')
        c += INTERVAL
