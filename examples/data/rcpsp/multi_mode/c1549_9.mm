************************************************************************
file with basedata            : c1549_.bas
initial value random generator: 1109767679
************************************************************************
projects                      :  1
jobs (incl. supersource/sink ):  18
horizon                       :  119
RESOURCES
  - renewable                 :  2   R
  - nonrenewable              :  2   N
  - doubly constrained        :  0   D
************************************************************************
PROJECT INFORMATION:
pronr.  #jobs rel.date duedate tardcost  MPM-Time
    1     16      0       22       10       22
************************************************************************
PRECEDENCE RELATIONS:
jobnr.    #modes  #successors   successors
   1        1          3           2   3   4
   2        3          1          15
   3        3          3           5   8  12
   4        3          3           6   7   9
   5        3          3           7  10  11
   6        3          1          17
   7        3          1          15
   8        3          2          10  13
   9        3          1          16
  10        3          1          16
  11        3          1          16
  12        3          1          13
  13        3          1          14
  14        3          2          15  17
  15        3          1          18
  16        3          1          18
  17        3          1          18
  18        1          0        
************************************************************************
REQUESTS/DURATIONS:
jobnr. mode duration  R 1  R 2  N 1  N 2
------------------------------------------------------------------------
  1      1     0       0    0    0    0
  2      1     2       0    6    8   10
         2     7       9    0    7    9
         3     8       0    4    7    9
  3      1     5       4    0    2    5
         2     7       3    0    2    3
         3     9       0    8    2    3
  4      1     2       4    0    7    4
         2     3       2    0    2    4
         3     3       0    2    2    4
  5      1     8       0    6    9    9
         2     9       2    0    7    4
         3    10       1    0    7    2
  6      1     2       0    6    6    9
         2     3       0    6    5    6
         3     6       0    5    4    6
  7      1     1       7    0    8    6
         2     6       0    8    6    5
         3     8       0    5    6    4
  8      1     3       0    6    8    7
         2     5       8    0    8    7
         3     7       0    5    6    4
  9      1     2       5    0    3   10
         2     4       4    0    2    3
         3     4       4    0    1    6
 10      1     7       8    0    6    3
         2     7       0   10    7    7
         3     7       0   10   10    4
 11      1     5       0    4    6    1
         2     6       3    0    3    1
         3     7       0    3    2    1
 12      1     7       6    0    9    8
         2     8       5    0    6    6
         3     9       3    0    6    6
 13      1     1       6    0    9    4
         2     2       4    0    8    3
         3     4       3    0    8    3
 14      1     4       0    9    2    8
         2     7       0    5    2    7
         3    10       0    5    1    3
 15      1     3       1    0   10    7
         2     6       0    7    9    5
         3    10       0    5    9    1
 16      1     2       8    0    5    7
         2     7       5    0    1    6
         3     7       6    0    3    5
 17      1     1       0    6    6    5
         2     2       0    4    6    5
         3    10       0    3    5    5
 18      1     0       0    0    0    0
************************************************************************
RESOURCEAVAILABILITIES:
  R 1  R 2  N 1  N 2
   10   12   99   96
************************************************************************
