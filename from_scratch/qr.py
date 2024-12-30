import numpy as np

def makesimilar(A):
    Q, R = np.linalg.qr(A)
    B = np.dot(R, Q)
    return B

def eig_qr(A):
    B = makesimilar(A)
    iters = 0
    feig = B[0, 0]
    leig = B[-1, -1]
    diff = 1
    while diff > 1e-4:
        B = makesimilar(B)
        iters += 1
        diff = abs(leig - B[-1, -1])+abs(feig - B[0, 0])
        feig = B[0, 0]
        leig = B[-1, -1]
    print(f"A_{iters} = \n{B}")
    eigs = [B[i, i] for i in range(len(B))]
    return eigs, iters

A = np.array([[8, 3, -1],
                [2, 8, 1],
                [0, 3, 9]])
eigs, iters = eig_qr(A)
print(f"A:\n {A}")
print(f"Eignevalues: {eigs}")
print(f"# of iterations: {iters}")