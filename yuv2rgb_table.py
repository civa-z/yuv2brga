import os,sys

print ("Y========\nshort Y_TABLE[256] = {"),


for i in range(0, 256):
    if i%16 == 0:
        print ("")
    if i <= 16:
        print (0),
        print (", "),
    else:
        print ((i-16)*298),
        print (", "),

print ("\n};\n\nshort BU_TABLE[256] = {"),

for i in range(0, 256):
    if i%16 == 0:
        print ("")
    print ((i-128)*409),
    print (", "),

print ("\n};\n\nshort GV_TABLE[256] = {"),

for i in range(0, 256):
    if i%16 == 0:
        print ("")
    print (-(i-128)*208),
    print (", "),

print ("\n};\n\nshort GU_TABLE[256] = {"),

for i in range(0, 256):
    if i%16 == 0:
        print ("")
    print (-(i-128)*100),
    print (", "),

print ("\n};\n\nshort RV_TABLE[256] = {"),

for i in range(0, 256):
    if i%16 == 0:
        print ("")
    print ((i-128)*209),
    print (", "),
print ("\n};"),
