# Week 9

## Part 1 : ANOVA

###########################

#ANOVA: to test mu1 = mu2 = mu3 = mu4

# 1 load data
setwd("F:/unimelb/2019S2/MAST90058 Elements of Statistics/mast20005-mast90058-semester2-2019/labs/data")

data <- read.table("corn.txt", header = TRUE)
data
Corn <- factor(data[, 1])
Yield <- data[, 2]
Corn

## [1] 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4
## Levels: 1 2 3 4

table(Corn)
## Corn
##1 2 3 4 
##4 4 4 4

Yield
## 68.82 86.84 90.16 61.58 76.99 75.69 78.84 73.51 74.30 77.87 80.65 74.57 78.73 76.18 83.58 70.75

tapply(Yield, list(Corn), mean) # group means
##  1       2       3       4 
## 74.7100 79.1450 83.3075 70.1025

boxplot(Yield ~ Corn)

#############################################3

# 2 ANOVA

m1 <- lm(Yield ~ Corn)
qqnorm(residuals(m1))

summary(m1)

## Call:
##lm(formula = Yield ~ Corn)

##Residuals:
##  Min      1Q  Median      3Q     Max 
##-8.5225 -3.0875 -0.0688  3.5606  7.6950 

##Coefficients:
##            Estimate Std. Error  t value
##(Intercept)   74.710      2.568  29.096
##Corn2          4.435      3.631   1.221
##Corn3          8.598      3.631   2.368
##Corn4         -4.607      3.631  -1.269
##              Pr(>|t|)    
##(Intercept) 1.69e-12 ***
##  Corn2         0.2454    
##Corn3         0.0356 *  
##  Corn4         0.2286    
##---
##  Signif. codes:  
##  0 ¡®***¡¯ 0.001 ¡®**¡¯ 0.01 ¡®*¡¯ 0.05 ¡®.¡¯
##0.1 ¡® ¡¯ 1

##Residual standard error: 5.135 on 12 degrees of freedom
##Multiple R-squared:  0.551,	Adjusted R-squared:  0.4387 
##F-statistic: 4.908 on 3 and 12 DF,  p-value: 0.01884

anova(m1)

## Analysis of Variance Table

##Response: Yield
##           Df   Sum Sq   Mean Sq  F value    Pr(>F)  
##  Corn      3   388.28   129.427   4.9078   0.01884 *
##  Residuals 12  316.46   26.372                  
##---
##  Signif. codes:  0 ¡®***¡¯ 0.001 ¡®**¡¯ 0.01 ¡®*¡¯ 0.05 ¡®.¡¯ 0.1 ¡® ¡¯ 1

##################################################

# 3 enter the command

pairwise.t.test(Yield, Corn, pool.sd = FALSE, p.adjust.method = "none")

# which gives the p-values for all possible paired t-tests. 
# (Note for advanced users: this version does not adjust for multiple testing.) 
# To confirm this table, try commands like:

t.test(Yield[Corn == 1], Yield[Corn == 3])

###################################################

# Part 2: Simulate the distribution of test statistics and p-values

x <- rnorm(5)
y <- rnorm(5)
test.result <- t.test(x, y, var.equal = TRUE)
names(test.result)

test.result$statistic


# 1.  To generate 1000 t-statistics from testing two groups 
#of 10 standard random normal numbers,
# replicate(times, vector)
ts <- replicate(1000,t.test(rnorm(5), rnorm(5), var.equal = TRUE)$statistic)

##########################################

# 2 the test statistics follows a t-distribution with 8 df
# one way to check is to 
#plot the (1) theoretical density of the t-statistic we should be seeing
# and (2) superimposing the density of our simulated statistics on top of it.

hist(ts, freq = FALSE, nclass = 25, col = "grey", ylim = c(0, 0.4),
     xlab = "T statistic", ylab = "Density") # histogram
lines(density(ts), lty = 2, lwd = 2) # smooth density estimate
curve(dt(x, df = 8), from = -4.5, to = 4.5, add = TRUE,
      col = "red", type = "l", lwd = 2) # theoretical density

######################################################

# 3 another way is QQ plot

qqplot(ts, rt(1000, df = 8), col = 4,
       xlab = "Test statistics",
       ylab = "Theoretical t-distribution (simulated)")
abline(0, 1)


########################################################

# 4 ???

probs <- c(0.9, 0.95, 0.99)

quantile(ts, probs)
## 90% 95% 99%
## 1.407738 1.849849 2.962914


qt(probs, df = 8)
## [1] 1.396815 1.859548 2.896459


###############################################

# 5.under H0, p-values for any test should form a uniform distribution between 0 and 1

pvals.welch <- replicate(1000, t.test(rnorm(10), rnorm(10))$p.value)
pvals.ttest <- replicate(1000, t.test(rnorm(10), rnorm(10),
                                      var.equal = TRUE)$p.value)
par(mfrow = c(1, 2))
hist(pvals.welch, freq = FALSE, col = 8, xlab = "p-values", main = "Welch")
hist(pvals.ttest, freq = FALSE, col = 8, xlab = "p-values", main = "Pooled")


probs <- c(0.5, 0.7, 0.9, 0.95, 0.99)

quantile(pvals.welch, probs)
## 50% 70% 90% 95% 99%
## 0.5013845 0.6961268 0.8985751 0.9497797 0.9890055

quantile(pvals.ttest, probs)
## 50% 70% 90% 95% 99%
## 0.5173447 0.7056634 0.8874657 0.9422718 0.9844300

# There is not that much of a dierence between theoretical and simulated quantiles for both tests.


#####################################################

# 6 Under H1, what is the distribution of p-value


pvals.welch <- replicate(1000, t.test(rnorm(10), rnorm(10, 1))$p.value)
pvals.ttest <- replicate(1000, t.test(rnorm(10), rnorm(10, 1),
                                      var.equal = TRUE)$p.value)
par(mfrow = c(1, 2))
hist(pvals.welch, freq = FALSE, col = 8, xlab = "p-values", main = "Welch")
hist(pvals.ttest, freq = FALSE, col = 8, xlab = "p-values", main = "Pooled")

quantile(pvals.welch, probs)
## 50% 70% 90% 95% 99%
## 0.03692803 0.10248368 0.29723121 0.43562115 0.73201027


quantile(pvals.ttest, probs)
## 50% 70% 90% 95% 99%
## 0.03474991 0.09975027 0.39903759 0.57613280 0.81187650


# The distribution is not uniform. Thus, the probability that the p-value is smaller than
#= 0:05 under the alternative hypothesis is higher than under the null hypothesis, and
#this eect is more pronounced as the true dierence X ???? Y moves away from zero.


###############################################################

# Part 3: Power comparisons by simulations


B <- 1000 # number of simulation runs
R <- 50 # number of power values
n <- 5 # sample sizes

delta.seq <- seq(-3, 3, length = R) # sequence of true differences
power.t <- numeric(R) # initialize power vectors
power.w <- numeric(R)


for (i in 1:R) {
delta <- delta.seq[i]
# Simulate B p-values for each test.
pvals.t <- replicate(B, t.test(rnorm(n), rnorm(n, delta),
                               var.equal = TRUE)$p.value)
pvals.w <- replicate(B, wilcox.test(rnorm(n), rnorm(n, delta),
                                    exact = FALSE)$p.value)
# Record the estimated power (proportion of rejections).
power.t[i] <- mean(pvals.t < 0.05)
power.w[i] <- mean(pvals.w < 0.05)

}


# Plot simulated power for t- and Wilcoxon tests.
plot(delta.seq, power.t, type = "l", ylim = c(0, 1),
     ylab = "Power", xlab = expression(delta))

lines(delta.seq, power.w, lty = 2, col = 2)
abline(v = 0, lty = 3)



#########################################################

# lab exercises

## Question 1

## (a) one way anova
x <- InsectSprays
x
model1 <- lm(count ~ factor(spray),InsectSprays)
anova(model1)

## Analysis of Variance Table
##
## Response: count
##       Df Sum Sq Mean Sq   F value   Pr(>F)
## spray 5 2668.8   533.77    34.702     < 2.2e-16 ***
## Residuals 66 1015.2 15.38
## ---
## Signif. codes: 0 

########################################################
##(b) boxplot

boxplot(count ~ spray,InsectSprays)

##############################################

## (c) sqrt(count) boxplot

boxplot((sqrt(count) ~ spray),InsectSprays)

model2 <- lm(sqrt(count) ~ factor(spray),InsectSprays)
anova(model2)

## Analysis of Variance Table
##
## Response: sqrt(count)
## Df Sum Sq Mean Sq F value Pr(>F)
## spray 5 88.438 17.6876 44.799 < 2.2e-16 ***
## Residuals 66 26.058 0.3948
## ---
## Signif. codes: 0 

#################################################

## question 2

x <- ToothGrowth
x

##(a)
model3 <- lm(len ~ factor(supp) + factor(dose),ToothGrowth)
anova(model3)

## Analysis of Variance Table
##
## Response: len
##              Df Sum Sq Mean Sq F value Pr(>F)
## factor(supp) 1 205.35 205.35 14.017 0.0004293 ***
## factor(dose) 2 2426.43 1213.22 82.811 < 2.2e-16 ***
## Residuals   56 820.43 14.65
## ---
## Signif. codes: 0 

#####################################################

## (b) is there an interaction

model4 <- lm(len ~ factor(supp) * factor(dose),ToothGrowth)
anova(model4)

## Analysis of Variance Table
##
## Response: len
##              Df Sum Sq Mean Sq F value Pr(>F)
## factor(supp) 1 205.35 205.35 15.572 0.0002312 ***
## factor(dose) 2 2426.43 1213.22 92.000 < 2.2e-16 ***
## factor(supp):factor(dose) 2 108.32 54.16 4.107 0.0218603 *
## Residuals 54 712.11 13.19
## ---
## Signif. codes: 0 

############################################################

## (c) draw an interaction plot

with(ToothGrowth,interaction.plot(supp,dose,len,col = "blue"))


boxplot(len ~ dose:supp,ToothGrowth, col = "lightblue")


###########################################################

## question 3













