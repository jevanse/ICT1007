import os
import subprocess

def main(): 
    # Dirty script to just run paper_5.c for all test cases
    files = [os.path.join("/home/hello/Desktop/ICT1007/test_related/testcases", file) for file in os.listdir("../test_related/testcases")]
    for file in files:
        subprocess.run(["/home/hello/Desktop/ICT1007/Shaun/paper_5", file])
    

if __name__ == '__main__':
    main()