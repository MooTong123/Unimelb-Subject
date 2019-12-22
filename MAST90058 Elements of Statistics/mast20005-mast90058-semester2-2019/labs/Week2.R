# Week 2 workshop

getwd()

setwd()

install.packages("triangle")

library(triangle)

##############################################
# 3.1 calculator

2 + 3 / 4 *3


log(25)

sqrt(4)

pi * 4 ^ 2

a <- pi * 4^2
a


# remove all variables from R's memory

rm(a)
rm(list = ls())


# 3.2 Vectors and matrices


x <- c(4.1,-1.3,2.5,-0.6,-21.7)
x

x <- c(1,3,9,10)

y <- c(30,35,4)
y

z <- c(x,y)
z
`
n <- 5

1:n -1
1: (n-1)


z[2]


z[1:4]
z[2]


seq(1,2,by = 0.1)

seq(1,2,length.out = 20)

seq(1,2,length.out = 21)


#Matrix
# nrow means one vector has 2 numbers
A <- matrix(c(2,-1,4,2,-1,3), nrow = 2)
A

# 1st row, 2st column
A[1,2]

# 2st column
A[,2]


A[1,]

mean(A)


# 3.3 Characters

u1 <- c("male","female")
u2 <- c("apple","pear","kiwi","orange")
u1
u2

# to combine numeric and character vector
# paste()

# pay attention to sep = ""
labels <- paste(c("X","Y"),1:10,sep = "")
labels








# 3.4 vectorisation

c(1,2) + c(2,5)


c(1,2,3) + c(2,5)

#[1] 3 7 5
#Warning message:
#  In c(1, 2, 3) + c(2, 5) :
#  longer object length is not a multiple of shorter object length





################################################
# 4 Functions
# basic important functions


# max(),min()
# range()
# sort()
# length()
# sum()
# prod()
# dim()
# mean()

mean(z)


# rnorm: create random samples from a normal distribution

z <- rnorm(10)
z

# standard situation
z <- rnorm(n = 10, mean = 0, sd =1)
z


# 4.1 getting help

help(boxplot)

example(boxplot)

#######################################################
# 5 create script to store your works

############################################################
#6 Graphics

x <- rnorm(100,mean=10,sd = 2)
y <- rnorm(100,mean = 0, sd = 4)
plot(x,y,xlab = "Name of variable x",ylab = "Name of variable y")


boxplot(x,y,names = c("x","y"))

hist(x,freq = FALSE, nclass = 10)

# logical; if TRUE, the histogram graphic is a representation of frequencies,
#the counts component of the result; 
# if FALSE, probability densities, component density, are plotted 
# (so that the histogram has a total area of one). 
# Defaults to TRUE if and only if breaks are equidistant 
# (and probability is not specified).


qqnorm(x)
qqline(x)


##################################################
# 7 data structures
#data frame

x <- rnorm(100)
y <- 2 * x + rnorm(100,0,0.8)
z <- 0.5 * x + rnorm(100,0,0.5)
t <- data.frame(x,y,z)
summary(t$x)

plot(t) #scatter plot

L <- list(one =1 ,two = c(1,2), five = seq(0,1,length.out = 5))
L


L$five + 10

#######################################################

# 8 reading and writing data

t <- data.frame(x = c(1,2,3),y = c(30,20,10))
t

# save files
write.table(t,file = "mydata.txt",row.names = TRUE)

#load files
t2 <- read.table(file = "mydata.txt")
t2


# read.csv() 
# read.dta()
#read.spss()


###################################################

# 9 Missing data

x <- c(rnorm(10),NA,rnorm(2))
x


min(a)

# If you do not care about missing data and want to compute the statistics with the values
# that are present, use the additional argument na.rm = TRUE:


min(x,na.rm = TRUE)

mean(x,na.rm = TRUE)


########################################################

# 10 conditional execution and loops


x <- rnorm(10)

if (mean(x) > median(x)){
  "The mean is greater than the median"
} else{
  "The mean is smaller than the median"
}


B <- 1000
n <- 5
xbar.seq <- 1:B
for (i in 1:B){
  sample <- rnorm(5)
  xbar.seq[i] <- mean(sample)
}

hist(xbar.seq)

######################################################

# 11 write your own functions

myfun <- function(x){
  y <- x^2
  return(y)
}

myfun(1.5)

x <- seq(-2,2,length.out =100)
plot(x,myfun(x))



# doesn't output the correct the answer
mymedian <- function(x){
  n <- length(x)
  m <- sort(x)[(n + 1) / 2]
  return(m)
}

# should consider the condition even and odd

mymedian1 <- function(x){
  n <- length(x)
  if(n %% 2 == 1){
    med <- sort(x)[(n+1) / 2]
  }else{
   middletwo <- sort(x)[(n/2) + 0:1]
   med <- mean(middletwo)
  }
  return(med)
}

x <- rnorm(10)
mymedian1(x)

median(x)



################################################

###exercises

######1##############



### not log10, ln = log
# sum(log10(x))
x <- 1:100
sum(log(x))

######
sum(log(1:100))


# for loop 
sum = 0
for(i in 1:100){
  sum = sum + log(i)
}
print(sum)


###2#######
######sqrt!!!##############
x <- rnorm(10000000, mean = 1, sd =sqrt(2))
mean(x^2) # 2.997537

y <- var(x) - (mean(x))^2 #2.998562

###########3#############
#quantile function
# 95 % CI
qnorm(0.025)
qnorm(0.025, lower.tail = FALSE)
qnorm(0.975)




#########4#################
exp1pdf <- function(x){
  if(x < 0){
    d = 0
  }else{
    d = exp(-x)
  }
  return(d)
}

exp1pdf(2)
dexp(2)

exp1pdf(-1)
dexp(-1)

##############5##################


# Tutorial question

# Number 6

a <- c(10.39,10.43,9.99,11.17,8.91,11.20,11.38,7.74,10.61,11.11)

quantile(a,type = 7)

quantile(a,type = 6)

boxplot(a, col = 8, horizontal = TRUE)


##############6####################

# number 7 question

x <- c(9.9,4.7,20.5,1.8,4.7,9.8,20.5,20.2,6.5,3.0)
hist(x,xlab = "Maximum rainfull", freq = FALSE, main = NULL,col = 8)



p <- 1:10 / 11
Finv <- -log(-log(p))
x.sorted <- sort(x)

plot(Finv,x.sorted, pch = 19, xlab = "EV quantiles", ylab = "Sample quantiles")

fit <- lm(x.sorted ~ Finv)
abline(fit)


coef(fit)

###############7####################

###?????


















