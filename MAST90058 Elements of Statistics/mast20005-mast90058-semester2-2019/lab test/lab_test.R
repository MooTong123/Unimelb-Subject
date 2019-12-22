
# Question 1

setwd("F:/unimelb/2019S2/MAST90058 Elements of Statistics/mast20005-mast90058-semester2-2019/lab test")
marks <- read.csv("marks.csv")
marks

s1 <- marks[,1]
s2 <- marks[,2]
s1
s2

# (a)
cor.test(s1,s2)

## 0.5666

# (b)
model <- lm(s2 ~ s1)
summary(model)

#alpha = -6.6517  beta = 0.8917 Y = -6.6517 + 0.8917 x wrong!!

# should be sigma #12.91

coef(model)
sigma(model)



# (c)
confint(model)

#                   2.5 %    97.5 %
#(Intercept) -47.8288934 34.525560
# s1            0.3323607  1.450973


# (d)

data1 <- data.frame(s1 = 75)
predict(model,newdata = data1,interval = "prediction",level = 0.8)

## 60.22333  42.83703 77.60964 


## (e)
model1 <- lm(s1 ~ s2,marks)
data2 <- data.frame(s2 = 70)
predict(model1,newdata = data2,interval = "prediction",level = 0.8)

# 77.16238 65.99842 88.32635

###################################################

## question 2

#(a)

yields <- read.delim("yields.txt")
yields

# summary(yields$machine = "A") ## wrong

summary(yields[yields$machine == "A","yield"])

# 18.90   30.40   34.50   33.93   37.35   48.60 


## (b)

m1 = yields[yields$machine == "A","yield"]
m1

m2 = yields[yields$machine == "B","yield"]
m2

t.test(m1,m2,conf.level = 0.95, var.equal = TRUE)

#0.1152 >0.05, fail to reject H0,  yields are roughly same


## (c)

wilcox.test(m1,m2)
## 0.04 <0.05, reject H0, yields are not the same


##(d)
 #no, t.test assume normal, wilcox test doesn't


##(e)
s <-sd(m1)
s

q <- qnorm(0.05,mean = 40, sd = s / sqrt(20))
q

p <- pnorm(q,mean = 35,sd = s / sqrt(20))
p


h <- read.table("HExer.txt",header= TRUE)
h

id <- h[,1]
id

HExer <- h[,2]
HExer

summary(HExer)

x.bar <- mean(HExer)
x.bar
s <- sd(HExer)
s

n <- length(HExer)
n

z <- (x.bar - 9) / (s / sqrt(n))
z


# alpha = 5%
c <- qt(0.975,19)
c


# H1: mu != 9
2 * (1 - pt(z,19))

# H1: mu > 9 
(1 - pt(z,19))

t.test(HExer,mu = 9,conf.level = 0.95,alternative = "two.sided")


sales <- read.csv("sales.csv")
sales


model <- lm(sales ~ factor(customers) * factor(days),sales)
anova(model)


c <- sales$customers
c

s <- sales$sales
s

d <- sales$days
d

model2 <- lm(sales ~ days,sales)
anova(model2)
summary(model2)

data1 <- data.frame(customers = 200)
predict(model2,newdata = data1,interval = "prediction",level = 0.95)

cor.test(c,d)

