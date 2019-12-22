# Week 8


# One Part:  Two-sample tests


x <- c(1612, 1352, 1256, 922, 1560, 1456, 2324)
y <- c(1082, 1300, 1092, 1040, 910 , 1248, 1092, 1040, 1092, 1288)

############################################################
# 1.  to test the null hypothesis H0 : mu X = mu Y
# assuming same variance
t.test(x, y, var.equal = TRUE)

##
## Two Sample t-test
##
## data: x and y
## t = 2.6641, df = 15, p-value = 0.01769
## alternative hypothesis: true difference in means is not equal to 0
## 95 percent confidence interval:
## 75.78143 682.27572
## sample estimates:
## mean of x mean of y
## 1497.429 1118.400
############################################################

# 2.  to test the variance

var.test(x, y)

##
## F test to compare two variances
##
## data: x and y
## F = 12.121, num df = 6, denom df = 9, p-value = 0.001445
## alternative hypothesis: true ratio of variances is not equal to 1
## 95 percent confidence interval:
## 2.805969 66.949239
## sample estimates:
## ratio of variances
## 12.121
########################################################

# 3  use the Welch's test
t.test(x,y)

##
## Welch Two Sample t-test
##
## data: x and y
## t = 2.2644, df = 6.6981, p-value = 0.05963
## alternative hypothesis: true difference in means is not equal to 0
## 95 percent confidence interval:
## -20.42556 778.48271
## sample estimates:
## mean of x mean of y
## 1497.429 1118.400
###############################################

# 4 to test normality
par(mfrow = c(1, 2))
qqnorm(x, main = "X", col = 4)
qqline(x, lty = 2)
qqnorm(y, main = "Y", col = 4)
qqline(y, lty = 2)

#########################################


# 5 two sample Wilcoxon test with normal approximation
# The option exact specifies whether we wish to use a normal approximation for the dis-
#tribution of the test statistic instead of the exact distribution

wilcox.test(x, y, exact = FALSE)

##
## Wilcoxon rank sum test with continuity correction
##
## data: x and y
## W = 59, p-value = 0.02142
## alternative hypothesis: true location shift is not equal to 0

#################################################

# 6 to test wilcoxon test that the outliers do not affect its result very much

y2 <- y
y2[5] <- 470
wilcox.test(x, y2, exact = FALSE)

## Wilcoxon rank sum test with continuity correction
##
## data:  x and y2
## W = 59, p-value = 0.02142
## alternative hypothesis: true location shift is not equal to 0

################################################

# Second Part: Goodness-of-fit test

################################################
# 1 fit the model. compute the sample mean and sd

setwd("F:/unimelb/2019S2/MAST90058 Elements of Statistics/mast20005-mast90058-semester2-2019/labs/data")
butterfat <- read.table("butterfat.txt")[, 1]
x.bar <- mean(butterfat)
s <- sd(butterfat)

###########################################

# 2 partition the data and compute observed frequencies

b <- c(0, seq(374, 624, 50), 1000) # class boundaries
O <- table(cut(butterfat, breaks = b)) # observed counts
O

##
## (0,374] (374,424] (424,474] (474,524] (524,574] (574,624]
##    5         9       16         22        15       13
## (624,1e+03]
##     10

##################################################

# 3 compute expected frequencies 

## prob <- rep(0, 7)
## prob[1] <- pnorm(b[2], x.bar, s) - pnorm(b[1], x.bar, s)
## ...
## prob[7] <- pnorm(b[8], x.bar, s) - pnorm(b[7], x.bar, s)
## E <- prob * length(butterfat)

prob <- rep(0, 7)
for (k in 1:7)
  prob[k] <- pnorm(b[k + 1], x.bar, s) - pnorm(b[k], x.bar, s)
E <- prob * length(butterfat) # expected frequencies

# then carry out the test

C <- sum((O - E)^2 / E) # chi-squared statistic

d1 <- length(O) # number of classes

qchisq(0.95, d1 - 3) # critical value
## [1] 9.487729


1 - pchisq(C, d1 - 3) # p-value
## [1] 0.9076048

cbind(O, E)
##           O       E
## (0,374]   5     5.222232
## (374,424] 9     9.042687
## (424,474] 16    15.768114
## (474,524] 22    20.020264
## (524,574] 15    18.509602
## (574,624] 13    12.460972
## (624,1e+03] 10  8.976128


#####################################

# 4 can also use chi-squared test 
chisq.test(O, p = prob)

##
## Chi-squared test for given probabilities
##
## data: O
## X-squared = 1.0144, df = 6, p-value = 0.9851

## but remember that the nal degrees of freedom should be k-p-1
## where k is the number of classes and 
## p the number of estimated parameters. 
## That means we need to recalculate
## the p-value using the given value of the statistic 
## and the correct degrees of freedom:

1 - pchisq(1.0144, d1 - 3)
## [1] 0.9076047

#########################################

# Third Part Chi-squared test for association

###########################################

# 1 enter the data


# byrow:logical. If FALSE (the default) 
# the matrix is filled by columns,
# otherwise the matrix is filled by rows
X.raw <- c(171, 93, 82, 62, 54, 11, 31, 55, 43, 11, 27, 57)
X <- matrix(X.raw, nrow = 4, byrow = TRUE) 
# 4 rows, fill in numbers by rows first
X
test <- chisq.test(X)
test

##
## Pearson's Chi-squared test
##
## data: X
## X-squared = 105.62, df = 6, p-value < 2.2e-16


#######################################

# 2 shows the expected frequencies

test$expected

##          [,1]     [,2]    [,3]
## [1,] 136.51363 113.67862 95.80775
## [2,] 50.10760  41.72597  35.16643
## [3,] 50.89670  42.38307  35.72023
## [4,] 37.48207  31.21234  26.30560


# In R, show how to compute the first cell 
# in the table of expected frequencies 
# and give the first term in the chi-squared statistic

E.1 <- sum(X[, 1]) * sum(X[1, ]) / sum(X)
E.1
## [1] 136.5136
O.1 <- 171
(O.1 - E.1)^2 / E.1
## [1] 8.712022


####################################################

# Workshop

#########################################

# Question1

x <- c(0,8,42) # observed value
prob <- c(0.1,0.2,0.7) #expected prob
chisq.test(x,p = prob)

##
## Chi-squared test for given probabilities
##
## data: x
## X-squared = 6.8, df = 2, p-value = 0.03337


###################################

# Question2 

x <- c(41.195, 39.485, 41.229, 36.840, 38.050, 40.890, 38.345,
       34.930, 39.245, 31.031, 40.780, 38.050, 30.906)

# wilcoxon test

wilcox.test(x, mu = 40, alternative = "less")

##
## Wilcoxon signed rank test with continuity correction
##
## data: x
## V = 18, p-value = 0.02955
## alternative hypothesis: true location is less than 40

## reject H0


# sigh test

binom.test(sum(x > 40),length(x),alternative = "less")

##
## Exact binomial test
##
## data: sum(x > 40) and length(x)
## number of successes = 4, number of trials = 13, p-value =
## 0.1334
## alternative hypothesis: true probability of success is less than 0.5

## 95 percent confidence interval:
## 0.0000000 0.5726193
## sample estimates:
## probability of success
## 0.3076923

## can not reject H0

##################################################

#Question 3

x = c(254,69,87,22) #observed
# ratios <- c(9/16, 3/16, 3/16, 1/16)
ratios <- c(9,3,3,1)
chisq.test(x,p = ratios,rescale.p = TRUE)

# rescale.p : a logical scalar; 
# if TRUE then p is rescaled (if necessary) to sum to 1. 
# If rescale.p is FALSE, and p does not sum to 1, an error is given

##
## Chi-squared test for given probabilities
##
## data: flies
## X-squared = 3.6461, df = 3, p-value = 0.3023

# can not reject H0



#########################################

# question 4

## two sample

nurses <- rbind(c(95, 36, 71, 21, 45, 32),
                c(53, 26, 43, 18, 32, 28))
chisq.test(nurses)

##
## Pearson's Chi-squared test
##
## data: nurses
## X-squared = 3.2305, df = 5, p-value = 0.6645

# reject H0












