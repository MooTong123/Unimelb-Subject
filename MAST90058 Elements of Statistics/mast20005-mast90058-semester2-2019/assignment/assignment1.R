# Problem 1

#input the data
x <- c(41300,40300,43200,41100,39300,42100,42700,41300,
       38900,41200,44600,42300,40700,43500,39800,40400)

#(a)
summary(x)
var(x)
sd(x)
IQR(x)

boxplot(x, col = 8, horizontal = TRUE)
hist(x)

#(b)
library(MASS)
normal.fit <- fitdistr(x, densfun = "normal")
normal.fit

#(c)

pdf <- function(a) dnorm(a, mean = mean(x), sd = sd(x))
hist(x, freq = FALSE, col = "gray", xlab = "distance travelled(km)")
curve(pdf, col = 1, lty = 1, lwd = 2, add = TRUE)

#d)

qqnorm(x)
qqline(x)




#number 4 
#c 
x <- c(0.27,3.30,4.58,2.61,0.38,3.77,1.11,1.15,4.11,2.10,
       0.07,1.74,2.11,12.79,1.85,0.30,0.34,1.31,0.14,0.74)

n <- length(x)
mu.hat <- mean(log(x))
lambda.hat <- mean((log(x)-mu.hat)^2)
lambda.hat
s <- sd(log(x))

a <- qchisq(0.025,n-1)
b <- qchisq(0.975,n-1)

CI <- c((n-1)* s^2 / b, (n-1)* s^2 / a)
CI

# ii
qqnorm(log(x))
qqline(log(x))












