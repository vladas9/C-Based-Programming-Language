## Launching the program

To start the program just run the makefile
And after run, type the path to file that you want to interpret

# Syntax
## Number declaration 
```
#i <name> // int
#d <name> // double
```

**example**
```
#i num1
#d speed
```

## Output
```
print(identifier or an expression)
```
**example:**
```
print(num1)

print(num1 + num2 * 8)
```

## Number operations
```
x = y
x + y 
x - y 
x * y 
x/y   
```
## bool operation
```
<
>
==
```

## loop
```
condition {

}    
```

**example**
```
num1 > num2{
    print(num1)
    num1 = num1 + 1
}

(num1 + 7) == (num2 * 5){
    print(num2)
    num1 = num2 + 5 * 9
}
```
## if statement
```
condition ?{    #then

}:{             # else

}
```
**example**
```
a > b?{
    print(a)
}:{
    print(b)
}
```

# Examples of programs

```
#i a
#i b

a = 5
b = 4

a > b?{
    print(a)
}:{
    print(b)
}
```

Output: 5

```
#i a
#i b
#d c

a = 12
b = 4
c = 0.5
a > b{
    b = b + c * 2
    c = c * 3
    print(b)
}

```
Output: 
5
8
17