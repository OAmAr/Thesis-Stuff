import os
import decimal
import subprocess


def drange(x,y,jump):
    x= decimal.Decimal(x); y =decimal.Decimal(y)
    while x < y:
        yield float(x)
        x += decimal.Decimal(jump)

step  = '.001'

bb84_1_file   = "data/bb84_N_1.csv"
bb84_2_file   = "data/bb84_N_2.csv"
bb84_3_file   = "data/bb84_N_3.csv"
bb84_4_file   = "data/bb84_N_4.csv"
bb84_5_file   = "data/bb84_N_5.csv"
bb84_6_file   = "data/bb84_N_6.csv"

state_1_file  = "data/6state_N_1.csv"
state_2_file  = "data/6state_N_2.csv"
state_3_file  = "data/6state_N_3.csv"

skqd_1_file   = "data/skqd_1.csv"
skqd_2_file   = "data/skqd_2.csv"
skqd_3_file   = "data/skqd_3.csv"
skqd_4_file   = "data/skqd_4.csv"
skqd_5_file   = "data/skqd_5.csv"
skqd_6_file   = "data/skqd_6.csv"

def bb84_test(filename, N, qx_lambda):
    with open(filename, 'w+') as f:
  #      f.write("Noise, Key-Rate")
        f.write("0,1\n")
        for Q in drange(.01, 1, step):
            command_string = "./bb84x {} {} {} 2> /dev/null".format(Q,qx_lambda(Q), N)
            command = command_string.split(' ')
            proc =  subprocess.run(command,\
                    cwd = os.path.dirname(os.path.realpath(__file__)), \
                    stdout=subprocess.PIPE).stdout.decode('utf-8')
            res = proc.split(' ')
            if (res[3] == 'inf' or float(res[3]) <= .00000001):
                break
            f.write("{},{}\n".format(Q,res[3]))
            print("BB84 with N = {} gave a key rate of {} for noise {}".format(N,res[3],Q))
    return

def state_test(filename, N):
    with open(filename, 'w+') as f:
 #       f.write("Noise, Key-Rate")
        f.write("0,1\n")
        for Q in drange(.01, 1, step):
            command_string = "./6state {} {} 2> /dev/null".format(Q,N)
            command = command_string.split(' ')
            proc =  subprocess.run(command,\
                    cwd = os.path.dirname(os.path.realpath(__file__)), \
                    stdout=subprocess.PIPE).stdout.decode('utf-8')
            res = proc.split(' ')
            if (res[3] == 'inf' or float(res[3]) <= .00000001):
                break
            f.write("{},{}".format(Q,res[3]))
            print("6-State with N = {} gave a key rate of {} for noise {}".format(N,res[3],Q))
    return

def skqd_test(filename, qf_lambda, qr_lambda, qx_lambda):
    with open(filename, 'w+') as f:
#        f.write("Noise, Key-Rate")
        f.write("0,1\n")
        for Q in drange(.01, 1, step):
            qf,qr = qf_lambda(Q), qr_lambda(Q); qx= qx_lambda(qf, qr)
            command_string = "./skqd {} {} {} 2> /dev/null".format(qf,qx,qr)
            print(command_string)
            command = command_string.split(' ')
            proc =  subprocess.run(command,\
                    cwd = os.path.dirname(os.path.realpath(__file__)), \
                    stdout=subprocess.PIPE).stdout.decode('utf-8')

            res = proc.split(' ')
            if (res[5] == 'inf' or float(res[5]) <= .00000001):
                break
            f.write("{},{}\n".format(Q,res[5]))
            print("Our protocol gave a key rate of {} for noise {}".format(res[5],Q))

def main():
    ident    = lambda q     : q
    stdqx    = lambda q     : q * (1 - q)
    bb84_test(bb84_1_file, 1, ident)
    bb84_test(bb84_2_file, 1, stdqx)
    bb84_test(bb84_3_file, 2, ident)
    bb84_test(bb84_4_file, 2, stdqx)
    bb84_test(bb84_5_file, 3, ident)
    bb84_test(bb84_6_file, 3, stdqx)
    
    state_test(state_1_file, 1)
    state_test(state_2_file, 2)
    state_test(state_3_file, 3)

    ident    = lambda q     : q
    double   = lambda q     : 2 * q

    avgqx    = lambda qf,qr : (qf+qr)/2
    standqx  = lambda qf,qr : qf * (1 - qr) + qr * (1 - qf)


    skqd_test(skqd_1_file, ident, ident, avgqx)
    skqd_test(skqd_2_file, ident, ident, standqx)


    skqd_test(skqd_3_file, ident, double, standqx)

    skqd_test(skqd_4_file, double, ident, standqx)

    skqd_test(skqd_5_file, double, ident, avgqx)
    skqd_test(skqd_6_file, ident, double, avgqx)


if __name__=='__main__':
    main()

