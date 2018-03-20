import time

labels = \
[       "Q",\
        "BB84 N = 1 Qx = Q",\
        "BB84 N = 1 Qx = Q(1-Q)",\
        "BB84 N = 2 Qx = Q",\
        "BB84 N = 2 Qx = Q(1-Q)",\
        "BB84 N = 3 Qx = Q",\
        "BB84 N = 3 Qx = Q(1-Q)",\
        "6 State N = 1",\
        "6 State N = 2",\
        "6 State N = 3",\
        "Our Qf=Qr=Qx=Q",\
        "Our Qf=Qr=Q, Qx = 2Q(1-Q)",\
        "Our Qf=Q, Qr=2Q, Qx = Qf(1-Qr)+Qr(1-Qf)",\
        "Our Qf=2Q, Qr=Q, Qx = Qf(1-Qr)+Qr(1-Qf)",\
        "Our Qf=2Q, Qr=Q, Qx = (Qf+Qr)/2",\
        "Our Qf=Q, Qr=2Q, Qx = (Qf+Qr)/2"\
]

filenames = \
[   "blank",\
    "bb84_N_1.csv",\
    "bb84_N_2.csv",\
    "bb84_N_3.csv",\
    "bb84_N_4.csv",\
    "bb84_N_5.csv",\
    "bb84_N_6.csv",\
    "6state_N_1.csv",\
    "6state_N_2.csv",\
    "6state_N_3.csv",\
    "skqd_1.csv",\
    "skqd_2.csv",\
    "skqd_3.csv",\
    "skqd_4.csv",\
    "skqd_5.csv",\
    "skqd_6.csv",\
]

with open('combined.csv', 'w+') as f:
    files = [open(filename, "r") for filename in filenames[1:]]
    print(",".join(["\"{}\"".format(label) for label in labels]))
    f.write(",".join(["\"{}\"".format(label) for label in labels])+'\n')
    while True:
        raw_lines = [line for line in [fil.readline() for fil in files] ]
        try:
            q = max([float(line.split(',')[0]) if line != "" else -1 for line in raw_lines])
        except ValueError:
            pass
            
        if q == -1:
            print("No entry")
            print(raw_lines)
            break
        print(str(q)+ "," + ",".join([line.split(',')[1].strip() if len(line) else ""  for line in raw_lines]))
        f.write(str(q)+","+ ",".join([line.split(',')[1].strip() if len(line) else ""  for line in raw_lines])+'\n')
        time.sleep(.001)
    for fil in files:
        fil.close()



