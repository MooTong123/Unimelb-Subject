# Week 4

# 1 Confidence Interval


#Normal N(0,1)
p <- c(0.9,0.95,0.975)
qnorm(p)


#N(5,3^2)
qnorm(p,5,3)


# t5
qt(p,5)

#X1 ^ 2
qchisq(p,1)



# x5 ^ 2
qchisq(p,5)



#F12,4
qf(p,12,4)

# read.delim() doesn't work'

setwd("F:/unimelb/2019S2/MAST90058 Elements of Statistics/mast20005-mast90058-semester2-2019/labs/data")


PTweight1 <- read.delim("PTweight.txt")
PTweight1
x <- PTweight1[1:5,2]
x # Null,because this data is a table, should use read.table

x <- read.delim("yields.txt")
mean(x[,1])


PTweight <- read.table("PTweight.txt")
x <- PTweight[1:5,2]
x


n <- length(x)
x.bar <- mean(x)
s <- sd(x)
t <- qt(0.95,n-1)
x.bar + c(-1,1) * t * s / sqrt(n)
  


# built-in fnction
t.test(x,conf.level = 0.90)


y <- PTweight[6:10,2]
y.bar <- mean(y)
s.p <- sqrt((4 * var(x) + 4 * var(y)) / 8) # poor sample sd
x.bar - y.bar + c(-1,1) * qt(0.975, df=8) * s.p * sqrt(1/5 + 1/5)


t.test(x,y,var.equal = TRUE)





invadopodia <- read.table("invadopodia.txt")
## the data from second column, when the condition == 1 or 2
x1 <- invadopodia[invadopodia$Condition == 1, 2]
x2 <- invadopodia[invadopodia$Condition == 2, 2]
x1
x2

x.bar1 <- mean(x1)
x.bar2 <- mean(x2)

prop.test(x = 6, n = 20, conf.level = 0.90, correct = FALSE)


x <- c(33.8, 32.2, 30.7, 35.4, 31, 30.3, 26.8, 33.2, 27.8, 27.2)
n <- length(x)

t.test(x,conf.level = 0.90)



























