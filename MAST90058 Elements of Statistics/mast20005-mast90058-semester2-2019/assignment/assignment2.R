
# Question 1
x <- c(6,5.7,5.8,6.5,7.0,6.3,5.6,6.1,5)

var(x)
sd(x)

t.test(x,conf.level = 0.95)



# Question 3
pres <- pressure$pressure
temp <- pressure$temperature


#a) define x and y 
y <- log(pres)
x <- 1 / (temp - 10)


#b) fit the model
fit <- lm(y ~ x)
summary(fit)


#c) plot diagnostic plots
par(mfrow = c(1, 2))
plot(fit, 1:2) 

#d) 95% CI
confint(fit)

#e) CI
newdata = data.frame(x = 1 / (70 - 10))
predict(fit,newdata,interval = "confidence", level = 0.95)

#f) PI
predict(fit,newdata,interval = "predict", level = 0.95)


qt(0.95,28)
qnorm(0.995)



#Question 6

# a) Type I error
ppois(3,2,lower.tail = FALSE)

# b) Type II error when lambda = 5
ppois(3,5,lower.tail = TRUE)


# c) Draw power curve of lambda between 2 to 10

K1 <- function(p)
  1 - ppois(3,p)

p <- seq(2,10,0.1)
K <- K1(p)
plot(p,K,type = "l", ylab = "Power Curve")

K1(3)
K1(4)
K1(2)
K1(1)
K1(1.5)
K1(1.6)
K1(1.4)
K1(1.3)
K1(1.35)
K1(1.36)
K1(1.37)
