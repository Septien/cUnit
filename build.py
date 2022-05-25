import os

def compile():
    cmd = "make clean LINUX=1"
    os.system(cmd)

    cmd = "make all LINUX=1 CTEST=1"
    os.system(cmd)
    
    cmd = "make all term RIOT=1 CTEST=1"
    os.system(cmd)

    print("\n\nTesting for LINUX.\n\n")
    cmd = "bin/linux-x86_64/cunit"
    os.system(cmd)

if __name__ == '__main__':
    compile()
