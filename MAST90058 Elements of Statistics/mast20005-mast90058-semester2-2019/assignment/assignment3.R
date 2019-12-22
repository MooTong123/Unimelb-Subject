## Aiisgnment 3

#############################

# Question 1

high_protein <- c(134,146,104,119,124,161,112,83,113,129,97,123)
low_protein <- c(80,118,101,85,107,132,94)

## part a: sigh test with alpha = 0.05

binom.test(sum(high_protein > 110),length(high_protein),alternative = "less")


## part b: Wilcoxon rank-sum test with alpha = 0.05

wilcox.test(high_protein,low_protein,alternative = "greater")


## part c: t test with alpha = 0.05

t.test(high_protein,low_protein,var.equal = TRUE,alternative = "greater")

var.test(high_protein,low_protein)


###################################################################

## Question 2 

# (a) 
x <- rep(0:4, c(10,24,10,6,3))
table(x)
x.bar <- mean(x)
x.bar

p0 <- dpois(0,x.bar)
p1 <- dpois(1,x.bar)
p2 <- dpois(2,x.bar)
p3 <- 1 - (p0 + p1 +p2)

p <- c(p0,p1,p2,p3)
y <- c(10,24,10,9)

chisq.test(y,p = p)
# X-squared = 3.118

1 - pchisq(3.118,2)

# fail to reject H0, follow Poission distribution


# (b)

x <- rbind(younger = c(10,24,10,9),
           older = c(7,22,18,15))
x

chisq.test(x)

#fail to reject H0, which means that independent



######################################################

## question 3

##create the table

## 0 in Location column means outer surburb
## 1 in Location column means inner surburb
## 2 in Location column means CBD

retail <- data.frame("Competitors" = c(3,3,3,2,2,2,1,1,1,0,0,0,
                                       3,3,3,2,2,2,1,1,1,0,0,0,
                                       3,3,3,2,2,2,1,1,1,0,0,0),
                     "Locations" = c(0,0,0,0,0,0,0,0,0,0,0,0,
                                     1,1,1,1,1,1,1,1,1,1,1,1,
                                     2,2,2,2,2,2,2,2,2,2,2,2),
                     "Sales" = c(270,310,220,290,350,305,
                                 446,487,500,440,428,530,
                                 410,305,450,382,320,380,
                                 598,480,510,470,415,400,
                                 180,290,330,220,170,260,
                                 290,283,260,246,275,330))
retail
model <- lm(Sales ~ factor(Competitors) + factor(Locations),retail)
anova(model)

model2 <- lm(Sales ~ factor(Competitors) * factor(Locations),retail)
anova(model2)

with(retail,interaction.plot(Locations,Competitors,Sales,col = "blue"))



