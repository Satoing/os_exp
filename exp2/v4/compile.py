#!/usr/bin/python3
import os

filePath = './src'
files = os.listdir(filePath)

for file in files:
    os.system(f"g++ -fpic -shared -o ./plugin/lib{file[:-4]}.so ./src/{file}")
    
os.system("g++ -o main main.cpp -ldl")
os.system("./main")
# os.system("rm main")