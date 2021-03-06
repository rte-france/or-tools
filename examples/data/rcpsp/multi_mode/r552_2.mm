************************************************************************
file with basedata            : cr552_.bas
initial value random generator: 842988278
************************************************************************
projects                      :  1
jobs (incl. supersource/sink ):  18
horizon                       :  142
RESOURCES
  - renewable                 :  5   R
  - nonrenewable              :  2   N
  - doubly constrained        :  0   D
************************************************************************
PROJECT INFORMATION:
pronr.  #jobs rel.date duedate tardcost  MPM-Time
    1     16      0       18        6       18
************************************************************************
PRECEDENCE RELATIONS:
jobnr.    #modes  #successors   successors
   1        1          3           2   3   4
   2        3          3           7  11  17
   3        3          3           8  10  14
   4        3          3           5  10  17
   5        3          3           6   7  15
   6        3          2          11  13
   7        3          2          12  14
   8        3          3           9  12  17
   9        3          2          13  15
  10        3          1          15
  11        3          2          12  14
  12        3          1          16
  13        3          1          16
  14        3          1          16
  15        3          1          18
  16        3          1          18
  17        3          1          18
  18        1          0        
************************************************************************
REQUESTS/DURATIONS:
jobnr. mode duration  R 1  R 2  R 3  R 4  R 5  N 1  N 2
------------------------------------------------------------------------
  1      1     0       0    0    0    0    0    0    0
  2      1     5       8    4    0    0    4   10   10
         2     7       0    0    0    0    3    7    9
         3     9       0    0    0    0    2    6    8
  3      1     1       0    5    0    0    8    6    6
         2     2       0    0    9    4    0    4    6
         3    10       8    5    9    3    0    3    5
  4      1     6       1    3    0    4    0    7    7
         2     8       0    0    6    0    3    5    7
         3     9       0    0    5    0    0    5    6
  5      1     2       7    7    0    0    0    9    7
         2     7       4    0    0    0    0    7    4
         3     8       2    3    0    0    0    7    3
  6      1     5       0    4    0    0   10    7    8
         2     9       0    0    0    7    0    7    8
         3    10       5    3    0    6    0    3    7
  7      1     5       0    0    4    7    0    5    3
         2     8       8    0    3    0    0    4    2
         3    10       6    3    3    0    8    3    2
  8      1     2       7    9    0    0    0    6    9
         2     7       7    6    7    5    0    4    8
         3    10       0    3    7    4    4    4    8
  9      1     1       0    6    9    0    0    8    8
         2     1       0    0    0    7    0    6    7
         3    10       9    0    0    6    7    2    5
 10      1     8       0    0    6    0    0    7    7
         2     9       0    1    0    0    7    6    6
         3    10       5    0    6    0    5    6    5
 11      1     1       0    8    3    6    0   10    6
         2     4       3    7    0    5    1    6    6
         3     5       1    7    0    3    0    4    5
 12      1     3       0    7    0   10    8    9    7
         2     4       9    0    0    7    0    8    7
         3     9       0    4    7    6    6    8    7
 13      1     4       7    0    9    0    6    6    9
         2    10       0    0    0    8    0    6    8
         3    10       6    3    7    0    0    6    6
 14      1     2       0    6    0    7    0    8    7
         2     3       0    6    0    7    0    5    6
         3    10       0    5    7    0    8    5    6
 15      1     2      10    6    8    1    5    4    8
         2     3      10    5    0    1    0    2    6
         3     5       9    4    8    1    4    1    3
 16      1     1       2   10    0    4    0    8    2
         2     1       2    0    2    0    0    8    2
         3     7       1   10    0    5    4    8    1
 17      1     1       6    4    4    0    0    8    6
         2     9       0    0    0    9    0    8    5
         3    10       1    0    1    0    0    8    2
 18      1     0       0    0    0    0    0    0    0
************************************************************************
RESOURCEAVAILABILITIES:
  R 1  R 2  R 3  R 4  R 5  N 1  N 2
   33   16   31   30   32  108  102
************************************************************************
