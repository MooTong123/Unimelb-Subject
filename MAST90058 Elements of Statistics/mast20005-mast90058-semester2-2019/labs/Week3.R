# Week 3 

getwd()
setwd("F:/unimelb/2019S2/MAST90058 Elements of Statistics/mast20005-mast90058-semester2-2019/labs/data")

tasmania <- read.csv("tasmania.csv")

dim(tasmania)


names(tasmania[,1:5])


year <- tasmania[,1]
s1 <- tasmania[,2]
s2 <- tasmania[,3]


summary(s1)
sd(s1)

IQR(s1)


quantile(s1,type = 6)

quantile(s1,type = 7)


summary(s2)


sd(s2, na.rm = TRUE)

IQR(s2,na.rm = TRUE)


#histograms
hist(s1,freq = FALSE, xlab = "Extreme rainfall (Burnie, Tas)", col = 8,nclass =5)
smooth.density = density(s1)
lines(smooth.density,lty = 2,lwd =2, col =2)


#boxplots compare two distributions
boxplot(s1,s2,horizontal = TRUE, names = c("Burnie Is","Cape Grim"),col = c("yellow","orange"))

#plots empirical cdfs

ecdf1 <- ecdf(s1)
ecdf2 <- ecdf(s2)
plot(ecdf1)
plot(ecdf2, col = 2, add = TRUE)

#QQ plot

qqnorm(s1,main = "Normal QQ plot for S1")
qqline(s1)

# Extreme Value distribution

Finv <- function(p) {-log(-log(p))}
p <- (1:20) / 21
y <- sort(s1) 
x <- Finv(p)

plot(x,y,ylab = "Sample quantiles", xlab = "EV quantiles")
fit <- lm(y ~ x)
abline(fit) # lines of best fit




###########################################################


# Distributions


help.search("distriution")

help(Distributions)


# d-    pdf or pmf
# p-    cdf
# q-    inverse cdf (quantiles)
# r-    randomly generated numbers


# dnorm  aussume (mean = 0. sd = 1)
# The function dnorm returns the value of the probability density function 
# for the normal distribution given parameters for x, ??, and ??. Some examples of using dnorm are below:

dnorm(0)

dnorm(0) * sqrt(2 * pi)

dnorm(0,mean = 4, sd = 2)

dnorm(c(-1,0,1))


x <- seq(-20,20,by = 0.1)
y <- dnorm(x)
plot(x,y)


y <- dnorm(x,mean = 2.5, sd = 0.5)
plot(x,y)


########################
# pnorm  cdf

pnorm(0)


pnorm(1)


pnorm(1,lower.tail = FALSE) #upper tail probability


pnorm(0,mean = 2, sd = 3)


x <- seq(-20,20,by = 0.1)
y <- pnorm(x)
plot(x,y)


y <- pnorm(x,mean = 3, sd = 4)
plot(x,y)


###############qnorm

qnorm(c(0.25, 0.5, 0.75), mean = 1, sd = 2) 
# quartiles for N(1,2)


x <- seq(0, 1, by = 0.05)
y <- qnorm(x)
plot(x, y)

y <- qnorm(x, mean = 3, sd = 2)
plot(x, y)

y <- qnorm(x, mean = 3, sd = 0.1)
plot(x, y)


###############rnorm

rnorm(4)
## [1] -0.08676448 0.62984032 -1.36913780 1.06512790
rnorm(4, mean = 3, sd = 3)
## [1] 2.205825 4.538623 4.304493 5.650095
rnorm(4, mean = 3, sd = 3)
## [1] -2.317260 4.099793 -2.630690 2.899554

y <- rnorm(200)
hist(y)

y <- rnorm(200, mean = -2)
hist(y)
qqnorm(y)
qqline(y)

#########################MLE

#MLE

mu.hat = mean(s1)
mu.hat
## [1] 48.84
n = length(s1)
sigma.hat = sqrt((n - 1) * var(s1) / n)
sigma.hat
## [1] 16.04514

# fitdistr() in MASS package
# three arguments:
#  1. data   2. pdf or pmf  3. starting point

library(MASS)
# Prepares the Gumbel pdf:
dgumbel <- function(x, mu, sigma)
  exp((mu - x) / sigma - exp((mu - x) / sigma)) / sigma
# Fits the Gumbel distribution
gumbel.fit <- fitdistr(x = s1, densfun = dgumbel,
                       start = list(mu = 50, sigma = 10))
gumbel.fit
## mu sigma
## 41.542604 12.326253
## ( 2.899086) ( 2.205542)

#For the Normal, log-Normal, geometric, exponential and Poisson distributions, \
# fitdistr() uses closed-form MLEs


normal.fit <- fitdistr(x = s1, densfun = "normal")
normal.fit
## mean sd
## 48.840000 16.045136
## ( 3.587802) ( 2.536959)

##########optim######

neg.llk <- function(theta) { # negative log-likelihood
  mu <- theta[1]
  sigma <- theta[2]
  out <- -sum(log(dgumbel(s1, mu, sigma)))
  return(out)
}

fit <- optim(c(50, 10), neg.llk) # fits MLEs
theta.hat <- fit$par # returns estimates
theta.hat
## [1] 41.53789 12.32450



# Write fitted normal and Gumbel pdfs
pdf1 <- function(x) dnorm(x, mean = mu.hat, sd = sigma.hat)
pdf2 <- function(x) dgumbel(x, mu = theta.hat[1], sigma = theta.hat[2])

# Plot data and fitted models
hist(s1, freq = FALSE, col = "gray", main = NULL, xlab = "x", xlim = c(0,100))
curve(pdf1, from = 0, to = 100, col = 2, lty = 2, lwd = 2, add = TRUE)
curve(pdf2, from = 0, to = 100, col = 1, lty = 1, lwd = 2, add = TRUE)


###########4  MM


x.bar <- mean(s1)
x2.bar <- mean(s1^2)
a <- 0.577215
b <- 1.978
sigma.tilde <- sqrt((x2.bar - x.bar^2) / (b - a^2))
sigma.tilde
## [1] 12.51076


mu.tilde <- x.bar - a * sigma.tilde
mu.tilde
## [1] 41.6186


###############################
#exercise

library(MASS)
library(evd)
tasmania <- read.csv("tasmania.csv")


#1######################

summary(tasmania[, c(7, 9, 30)])

par(mar = c(4, 6, 1, 1)) # adjust margins to fit axis labels
boxplot(tasmania[, c(7, 9, 30)], col = 8, horizontal = TRUE, las = 1)

##############2############

par(mar = c(4, 14, 1, 1))
boxplot(tasmania[, -1], col = 8, horizontal = TRUE, las = 1)


############3#################

hist(tasmania[, 33], 10, col = 8, main = names(tasmania)[33])

hist(tasmania[, 53], 10, col = 8, main = names(tasmania)[53])


dgumbel <- function(x, mu, sigma)
  exp((mu - x) / sigma - exp((mu - x) / sigma)) / sigma

start1 <- list(mu = 50, sigma = 10) #£¿£¿£¿
fitdistr(tasmania[, 33], densfun = dgumbel, start = start1)

## mu sigma
## 74.389990 32.104613
## ( 7.519231) ( 5.825075)   £¿£¿£¿£¿


fitdistr(tasmania[, 53], densfun = dgumbel, start = start1)
## Error in fitdistr(tasmania[, 53], densfun = dgumbel, start = start1): 'x'
## contains missing or infinite values

fitdistr(tasmania[-1, 53], densfun = dgumbel, start = start1)

## Warning in log(dens(parm, ...)): NaNs produced
## mu sigma
## 78.659729 20.726543
## ( 4.976391) ( 4.026766)


#############4################

a <- 0.577215
b <- 1.978

sigma.mm <- function(x)
  sqrt((mean(x^2) - mean(x)^2) / (b - a^2))

mu.mm <- function(x)
  mean(x) - a * sigma.mm(x)


B <- 10000 # number of simulated samples
# Initialise vectors to hold estimates from the simulations.


sigma.mms <- numeric(B)
mu.mms <- numeric(B)


# Run simulations.
for (i in 1:B) {
  x <- rgumbel(20, 50, 10)
  sigma.mms[i] <- sigma.mm(x)
  mu.mms[i] <- mu.mm(x)
}


# Evaluate simulation results.

mm.mean <- c(mean(mu.mms), mean(sigma.mms))
mm.var <- c( var(mu.mms), var(sigma.mms))
true <- c(50, 10)
bias.estimate <- mm.mean - true

mm.mean
## [1] 50.235427 9.477394

bias.estimate
## [1] 0.2354274 -0.5226064

mm.var
## [1] 5.699199 4.629722


###############################
##5

###rep!!
d <- rep(0:6, c(5, 7, 12, 9, 5, 1, 1)) # create vector with the data
d
## [1] 0 0 0 0 0 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3
## [34] 4 4 4 4 4 5 6


table(d) # table of counts
## d
## 0 1 2 3 4 5 6
## 5 7 12 9 5 1 1


mean(d) # MLE
## [1] 2.225


lambda <- seq(1, 3.5, length.out = 100)
likelihood <- numeric(100) #create a 100 length vector
for (i in 1:100)
  likelihood[i] <- prod(dpois(d, lambda[i]))
# dpois(x, lambda, log = FALSE)  poisson distribution


plot(lambda, likelihood, type = "l", lwd = 2, col = 4)


###################6##################


d <- c(6.1, -1.1, 3.2, 0.7, 1.7) # data from the hint
theta <- seq(-2, 6.5, length.out = 100)
llh <- numeric(100)

for (i in 1:100)
  llh[i] <- -length(d) * log(2) - sum(abs(d - theta[i]))

plot(theta, llh, type = "l", lwd = 2, col = 4)
rug(d, col = 4) # plot the location of data points, on the x-axis




###########7###################


x <- c(0.0256, 0.3051, 0.0278, 0.8971, 0.0739,
       0.3191, 0.7379, 0.3671, 0.9763, 0.0102)
sum(log(x))
## [1] -18.2063
sum(x)
## [1] 3.7401


ddistr <- function(x, theta)
  theta * x^(theta - 1)

theta <- seq(0, 1, length.out = 100)

likelihood <- numeric(100)

for (i in 1:100)
  likelihood[i] <- prod(ddistr(x, theta[i]))

plot(theta, likelihood, type = "l", lwd = 2, col = 4)

-length(x) / sum(log(x)) # MLE using formula
## [1] 0.5492604


fitdistr(x, densfun = ddistr, start = list(theta = 0.5))

## Warning in stats::optim(x = c(0.0256, 0.3051, 0.0278, 0.8971, 0.0739, 0.3191,
##: one-dimensional optimization by Nelder-Mead is unreliable:
  ## use "Brent" or optimize() directly
  ## theta
  ## 0.5492188
  ## (0.1736776)
  # Don't worry about the above warning.
# The optimisation is working fine in this case.



##########8#####################

curve(dexp(x, rate = 1/3), 0, 9, ylim = c(0, 0.4), lwd = 2, col = 4)

B <- 10000
xbars <- numeric(B)

for (i in 1:B) {
  x <- rexp(20, rate = 1/3)
  xbars[i] <- mean(x)
}

c(mean(xbars), var(xbars)) # estimates from simulations
## [1] 2.9987859 0.4577881

c(3, 3^2 / 20) # true values
## [1] 3.00 0.45


fitdistr(c(3.5, 8.1, 0.9, 4.4, 0.5), "exponential")
## rate
## 0.2873563
## (0.1285097)

1 / 0.2873563 # MLE for theta rather than the rate parameter
## [1] 3.48

























