# Demo_Optimization
A small demo to show the progress of optimization in a parser

## Basic Information



### Function

Match lines with offset and assembly code in the file given in the dataset, and then store them.

### Environment

A Quad-Core Intel Core i5 CPU @ 1.4GHz with 16GB RAM on macOS Monterey.

### DataSet

Data Set is too large to push to github.

The files could be found in this link:

https://drive.google.com/drive/folders/1bNLpeReT0rvGljnwWtG9r6twVEbZiEgU?usp=sharing



### How To Run

1. Put files in the dataset above into the path ***demo/dataset***.

2. Enter the path ***demo/ExecutableFile***

3. Use command ***time*** to get the running time of each function in each data scale like :

   ```bash
   time ./1G_match_noRegex_OpenMP
   ```

   




## Process of Optimization



Originally, it is implemented by **match_regex()** with regex functions.

The running time is about **293 seconds** for 1GB file.

Ways to optimize could be divided by two types: parallel ways and non-parallel ways.

### Parallel Ways

Firstly, I optimized the demo by parallel ways.

**(1)	Multiple Threads**

It is easy to find that the progress of matching each line by regex in the ***for*** loop is so suitable to be implemented by multple threads, because each loop is independent to others.

I implemented **match_regex_multiThread** function by creating threads by myself. I set 4 threads to match the strings. In order to avoid the recourse competition, I created local vector to store the results for each thread and combined them in the end by mutex lock.

In this part, the running time is about **134 seconds** for 1GB file.

**(2)	OpenMP**

Then I think OpenMP may be better than the multi-thread function written by me. So I tried it in **match_regex_OpenMP** function.

In this part, the running time is nearly the same as the multi-thread one.

**(3)	SIMD**

After implementing multi-thread ways in ***for*** loop, I began to consider optimizing regex procedure itself. Considering that it may be hard to matching one string in a multi-thread way, I think SIMD may be suitable for it.

I used an API named rejit based on SIMD to optimize the regex function. This part is implemented in ***include/help_func.hpp***. Using SIMD or not could be determined by ***#define SIMD_MATCH***. 

After testing the program after using SIMD in regex function, the running time is decreased over 80%. (When the input file size is 17M).

Due to the result of this part of experiment,  I utilized SIMD in regex to match strings in multi-thread and OpenMP ways. Then I collect the running time to construct final experiment results.

### Non-parallel Ways

After optimizing the demo by multi-thread ways, I focus on non-multi-thread part.

**(1)	getline() to fgets()**

***getline*** function of C++ took up large part of time, which was found by profiler. After searching, I learned that using ***fgets()*** of C by buffer could be quicker. Then I transformed the reading way to **fgets()** and gained a good improvement. 

Thus, I utilized **fgets()** to all methods too.

**(2)	Replace regex matching**

By analyzing the complexity of regular expression, which is higher than linear ways (match and extract substring by c++ string function), and using CLion profiler to analyze the program, I guess that the performance may be much better after replacing regex ways by string functions. Of course, the standard format of the input file is required.

The result of experiment could also prove this point. In order to make the comparison more meaningful, I change the requirement of the function to: Match offset and its corresponding assembly code in the file, and then store them. 

Unfortunately, I didn't find methods in rejit API which could complete this requirement. So in this part, the SIMD way could not be compared. The visulized result is followed. As we can see, after replacing regex, the running time decreased so much.

![image-20221015000059647](images/image-20221015000059647.png)



However, if the input file is not standard enough, there may be wrong match results. Thus, the performance of methods without regex would not be showed in the final experiment result. 

Though this method is not safe and general enough in many situations, we still could use more determine statements to avoid invalid regex match.



## Visualized Results of Experiment

Finally, after trying such optimization methods, I redesigned the experiment to show the progress of optimization. In order not to make the results too complex, I only maintained the results of main methods to show. (For example, methods using getline() function and multi-thread methods without SIMD are not showed).

Through the visulized results below, we could found that SIMD is very powerful while being utilized in regex match. Multi-thread methods and OpenMP are useful too, especially when the file size is large. It seems that OpenMP is not more powerful than multi-thread methods writen by myself in this situation.

![image-20221015231259588](images/image-20221015231259588.png)

![image-20221015230608812](images/image-20221015230608812.png)



## Summary

### What I did

* Utilized parallel and non-parallel ways to optimize the regex demo.

### What I learnt

* Learnt more ideas to optimize a program.
* Understand some basic knowledge more deeply. For instance, I spent long time debugging multi-thread function written by myself due to something about threads and mutex.
* Found that my former projects could be optimized better. For instance, the problem of competition in my former projects was not solved as well as this demo.

### What I want to do in the Future

* There is still something confusing me in SIMD part. I think the function of regex using SIMD could be more powerful (maybe in other API). I haven't studied the source code of API very well.