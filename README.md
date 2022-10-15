# Demo_Optimization
A small demo to show the progress of optimization in a parser

## Basic Information



### Function

Match offset and its corresponding assembly code in the file, and then store them.

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

Firstly, I optimized the demo by multi-thread ways.

(1)	Multiple Thread

I implemented **match_regex_multiThread** function by creating threads by myself. I set 4 threads to match the strings.

In this part, the running time is about **134 seconds** for 1GB file.

(2)	OpenMP

Then I think OpenMP may be better than the multi-thread function written by me. So I tried it in **match_regex_OpenMP** function.

In this part, the running time is nearly the same as the multi-thread one.



### Non-parallel Ways

After optimizing the demo by multi-thread ways, I focus on non-multi-thread part.

By analyzing the complexity of regular expression, which is higher than linear ways (match and extract substring by c++ string function), and using CLion profiler to analyze the program, I guess that the performance may be better after replacing regex ways.





## Experiment Visualized Results

![image-20221015000059647](images/image-20221015000059647.png)