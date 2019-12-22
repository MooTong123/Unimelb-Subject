#!/usr/bin/env python 
# -*- coding: utf-8 -*-
# @Time    : 11/10/19 2:03 pm
# @Author  : Jianhongl
# @Site    : 
# @File    : Q_learning_agent.py.py
# @Software: PyCharm
# myTeam.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from captureAgents import CaptureAgent
from baselineTeam import ReflexCaptureAgent
import random, time, util
from game import Directions
import game
import copy


#################
# Team creation #
#################
def createTeam(firstIndex, secondIndex, isRed,
               first='Defender', second='offensiveAgent'):
    """
  This function should return a list of two agents that will form the
  team, initialized using firstIndex and secondIndex as their agent
  index numbers.  isRed is True if the red team is being created, and
  will be False if the blue team is being created.

  As a potentially helpful development aid, this function can take
  additional string-valued keyword arguments ("first" and "second" are
  such arguments in the case of this function), which will come from
  the --redOpts and --blueOpts command-line arguments to capture.py.
  For the nightly contest, however, your team will be created without
  any extra arguments, so you should make sure that the default
  behavior is what you want for the nightly contest.
  """

    # The following line is an example only; feel free to change it.
    return [eval(first)(firstIndex), eval(second)(secondIndex)]


##########
# Agents #
##########
class DummyAgent(CaptureAgent):
    """
    A Dummy agent to serve as an example of the necessary agent structure.
    You should look at baselineTeam.py for more details about how to
    create an agent as this is the bare minimum.
    """

    def registerInitialState(self, gameState):
        """
        This method handles the initial setup of the
        agent to populate useful fields (such as what team
        we're on).

        A distanceCalculator instance caches the maze distances
        between each pair of positions, so your agents can use:
        self.distancer.getDistance(p1, p2)

        IMPORTANT: This method may run for at most 15 seconds.
        """

        '''
        Make sure you do not delete the following line. If you would like to
        use Manhattan distances instead of maze distances in order to save
        on initialization time, please take a look at
        CaptureAgent.registerInitialState in captureAgents.py.
        '''
        CaptureAgent.registerInitialState(self, gameState)

        '''
        Your initialization code goes here, if you need any.
        '''
        self.start = gameState.getAgentPosition(self.index)
        self.food = self.getFood(gameState).asList()
        self.width = gameState.data.layout.width
        self.height = gameState.data.layout.height
        self.mid_x = self.get_middle_line()
        self.enemiesIndices = self.getOpponents(gameState)
        self.walls = gameState.getWalls().asList()
        self.legal_positions = self.get_legal_position(gameState)
        self.enemiesIndices = self.getOpponents(gameState)
        self.teammatesIndices = self.getTeam(gameState)
        self.homeboundary = self.get_home_pos(gameState)
        self.distancer.getMazeDistances()
        self.lastObservedFood = None
        self.targetPosition = None
        self.capsulelist = self.getCapsules(gameState)

    def chooseAction(self, gameState):
        """
        Picks among the actions with the highest Q(s,a).
        """
        actions = gameState.getLegalActions(self.index)
        return random.choice(actions)

    def getSuccessor(self, gameState, action):
        """
        Finds the next successor which is a grid position (location tuple).
        """
        successor = gameState.generateSuccessor(self.index, action)
        pos = successor.getAgentState(self.index).getPosition()
        if pos != util.nearestPoint(pos):
            # Only half a grid position was covered
            return successor.generateSuccessor(self.index, action)
        else:
            return successor

    def getQValue(self, gameState, action):
        """
        Computes a linear combination of features and feature weights
        """
        features = self.getFeatures(gameState, action)
        weights = self.getWeights()
        return features * weights

    def getFeatures(self, gameState, action):
        """
        Returns a counter of features for the state
        """
        features = util.Counter()
        return features

    def getWeights(self, gameState, action):
        """
        Normally, weights do not depend on the gamestate.  They can be either
        a counter or a dictionary.
        """
        return {'successorScore': 1.0}
    """
    some useful functions
    """

    def get_middle_line(self):
        if self.red:
            mid_x = (self.width - 2) / 2
        else:
            mid_x = (self.width - 2) / 2 + 1
        return mid_x

    def get_home_pos(self, gameState):
        homeboundary = []
        mid_x = int(self.get_middle_line())
        for i in range(self.height):
            if not gameState.hasWall(mid_x, i):
                homeboundary.append((mid_x,i))
        return homeboundary

    def get_scaredtime(self, gameState):
        return None

    def get_position(self,gameState):
        return gameState.getAgentState(self.index).getPosition()

    def get_legal_position(self, gameState):
        return [pos for pos in gameState.getWalls().asList(False) if pos[1] > 1]
    """
    Heuristic functions
    """
    def nullHeuristic(self, state, gameState):
        return 0
    def GhostHeuristic(self, state, gameState):
        """
        This heuristic is to stay away from the ghost
        :return: heuristic value
        """
        myposition = state
        enemies = self.getOpponents(gameState)
        enemy_pos = []
        ghost = []
        heuristic = 0
        for enemy in enemies:
            enemy_pos.append(gameState.getAgentState(enemy))
        for elem in enemy_pos:
            if not elem.isPacman and elem.getPosition() != None:
                ghost.append(elem)
        if len(ghost) > 0:
            distance = 99999
            for g in ghost:
                dis_to_ghost = self.getMazeDistance(myposition, g.getPosition())
                if dis_to_ghost < distance:
                    distance = dis_to_ghost
            if distance < 2:
                heuristic = distance + 99999
        return heuristic

    def SuperHeuristic(self, state, gameState):
        myposition = state
        enemies = self.getOpponents(gameState)
        enemy_pos = []
        ghost = []
        heuristic = 0
        for enemy in enemies:
            enemy_pos.append(gameState.getAgentState(enemy))
        for elem in enemy_pos:
            if elem.isPacman and elem.getPosition() != None:
                ghost.append(elem)
        if len(ghost) > 0:
            distance = 99999
            for g in ghost:
                dis_to_ghost = self.getMazeDistance(myposition, g.getPosition())
                if dis_to_ghost < distance:
                    distance = dis_to_ghost
            if distance == 1:
                heuristic = distance + 99999
        return heuristic

    def CapsuleHeuristic(self, state, gameState):
        myposition = state
        capsules = self.getCapsules(gameState)
        heuristic = 0

        if len(capsules) > 0:
            distance = 99999
            for capsule in capsules:
                dis_to_capsule = self.getMazeDistance(myposition, capsule)
                if dis_to_capsule < distance:
                    distance = dis_to_capsule
                if distance <= 1:
                    heuristic = distance + 99999
        return heuristic
    """
    algorithm
    """
    def aStar_search(self, problem, gameState, heuristic = nullHeuristic):
        """Search the node that has has the weighted (x 2) lowest combined cost and heuristic first."""
        "*** YOUR CODE HERE FOR TASK 2 ***"

        """Initialize the open and closed set."""
        from util import PriorityQueue as Queue
        start_state = self.getCurrentObservation().getAgentState(self.index).getPosition()
        openedlst = Queue()
        closedlst = []
        best_g = {}
        best_g[start_state] = 0
        openedlst.push((start_state,[]), 0)
        while not openedlst.isEmpty():
            currentNode, currentPath = openedlst.pop()
            g = len(currentPath)
            if currentNode not in closedlst or g < best_g[currentNode]:
                closedlst.append(currentNode)
                best_g[currentNode] = g
                # print("currentNode",currentNode)
                if problem.isGoalState(currentNode):
                    # print("currentPath",currentPath)
                    return currentPath
                successors = problem.getSuccessors(currentNode)
                for expandNode, action, stepCost in successors:
                    newPath = currentPath + [action]
                    priority = len(newPath) + heuristic(expandNode, gameState)
                    openedlst.push((expandNode, newPath), priority)
                    if expandNode not in closedlst:
                        best_g[expandNode] = len(newPath)
        return None


class offensiveAgent(DummyAgent):
    def registerInitialState(self, gameState):
        DummyAgent.registerInitialState(self,gameState)
        self.alpha = 0.9
        self.eps = 0.9
        self.counter = 0
        self.discount = 0.9
        self.middle_point = self.homeboundary[int(len(self.homeboundary) / 2)]
        self.foodlist = self.getFoodYouAreDefending(gameState).asList() + self.getCapsulesYouAreDefending(gameState)
        self.weightsO = util.Counter()
        self.weightsO['successorScore'] = 10
        self.weightsO['distanceToFood'] = 100
        self.weightsO['distanceToEnemy'] = -100
        self.weightsO['distanceToHomebondary'] = 1
        self.weightsO['distanceToCapsule'] = 100
        # self.weightsO['STOP'] = 2.5394


    def chooseAction(self, gameState):
        actions = gameState.getLegalActions(self.index)

        # You can profile your evaluation time by uncommenting these lines
        values = [self.getQValue(gameState, a) for a in actions]

        maxValue = max(values)
        bestActions = [a for a, v in zip(actions, values) if v == maxValue]

        foodLeft = len(self.getFood(gameState).asList())

        if foodLeft <= 2:
            bestDist = 9999
            for action in actions:
                successor = self.getSuccessor(gameState, action)
                pos2 = successor.getAgentPosition(self.index)
                dist = self.getMazeDistance(self.start, pos2)
                if dist < bestDist:
                    bestAction = action
                    bestDist = dist
        else:
            if util.flipCoin(self.eps) and len(bestActions) > 0:
                bestAction = random.choice(bestActions)
            else:
                bestAction = random.choice(actions)

        # Get the reward for this action
        reward = self.get_reward(gameState, bestAction)
        # successor = self.getSuccessor(gameState, bestAction)
        successor = gameState.generateSuccessor(self.index,bestAction)

        #Update the Qvalue table
        self.update(gameState, bestAction, successor, reward)
        return bestAction


    def getWeights(self):
        return self.weightsO

    def getFeatures(self, gameState, action):
        features = util.Counter()
        successor = self.getSuccessor(gameState, action)
        foodList = self.getFood(successor).asList()
        myposition = successor.getAgentState(self.index).getPosition()
        hasEnemies = False
        # get teammate position
        for mem in self.teammatesIndices:
            if mem is not self.index:
                teamposition = gameState.getAgentState(mem).getPosition()
         # get enemies positions
        for enemy in self.enemiesIndices:
            enemy_pos = gameState.getAgentState(enemy).getPosition()
            if enemy_pos:
                dis_to_me = self.getMazeDistance(enemy_pos,myposition)
                dis_to_teammate = self.getMazeDistance(enemy_pos, teamposition)
                if dis_to_me < dis_to_teammate and dis_to_me < 5:
                    hasEnemies = True
        # get the features: successorScore
        if len(foodList) != 0:
            features['successorScore'] = 1/len(foodList)
        # get the features: distanceToFood
        min_distance = 99999
        if len(foodList) > 0:
            for food in foodList:
                distance = self.getMazeDistance(myposition, food)
                if min_distance > distance:
                    min_distance = distance
            features['distanceToFood'] = 1 / min_distance + 1

        # get the features: distanceToEnemy
        if hasEnemies:
            distance_to_enemy = []
            enemiesIndices = self.getOpponents(gameState)
            for enemy in enemiesIndices:
                enemy_position = gameState.getAgentState(enemy).getPosition()
                if enemy_position != None:
                    distance_to_enemy.append(self.getMazeDistance(enemy_position, myposition))
            if len(distance_to_enemy) > 0:
                print(distance_to_enemy)
                min_distance_to_enemy = min(distance_to_enemy)
                features['distanceToEnemy'] = 1 / min_distance_to_enemy + 1

        # get the features: distanceToHomebondary
        if gameState.getAgentState(self.index).numCarrying > 3:
            distance_to_home = []
            homeboundary = self.get_home_pos(gameState)
            for home in homeboundary:
                distance_to_home.append(self.getMazeDistance(home, myposition))
            min_distance_to_home = min(distance_to_home)
            if min_distance_to_home != 0:
                features['distanceToHomebondary'] = 1 / min_distance_to_home + 1
            else:
                features['distanceToHomebondary'] = 1

        # get the features:distancetocapsule
        if len(self.getCapsules(gameState)) != 0:
            distance_to_capsule = []
            capsulelist = self.getCapsules(gameState)
            for capsule in capsulelist:
                distance_to_capsule.append(self.getMazeDistance(capsule, myposition))
            if len(distance_to_capsule) > 0:
                min_distance_to_capsule = min(distance_to_capsule)
                if min_distance_to_capsule != 0:
                    features['distanceToCapsule'] = 1 / min_distance_to_capsule + 1
                else:
                    features['distanceToCapsule'] = 1
        # get the features: Stop
        return features

    def getMaxvalue(self, successor):
        actions = successor.getLegalActions(self.index)
        values = [self.getQValue(successor, a) for a in actions]
        maxValue = max(values)
        return maxValue

    def get_reward(self, gameState, bestAction):
        reward = 0
        myposition = gameState.getAgentState(self.index).getPosition()
        successor = gameState.generateSuccessor(self.index, bestAction)
        newPosition = successor.getAgentState(self.index).getPosition()
        foodList = self.getFood(gameState).asList()
        capsuleslist = self.getCapsules(gameState)
        x, y = newPosition
        newPosition = (int(x), int(y))
        # reward for the score:
        if gameState.getScore() < successor.getScore():
            reward +=100
        # reward for the food:
        distance_to_food1 = []
        distance_to_food2 = []
        if len(self.food) > 0:
            for food in foodList:
                distance_to_food1.append(self.getMazeDistance(myposition, food))
        if len(distance_to_food1) > 0:
            min_distance1 = min(distance_to_food1)
        else:
            min_distance1 = 0

        if len(self.food) > 0:
            for food in foodList:
                distance_to_food2.append(self.getMazeDistance(newPosition, food))
        if len(distance_to_food2) > 0:
            min_distance2 = min(distance_to_food2)
        else:
            min_distance2 = 0

        if min_distance1 > min_distance2:
            reward += 100
        else:
            reward -= 100
        # reward for the enemy:
        # distance_to_enemy1 = []
        # distance_to_enemy2 = []
        # enemyIndices = self.getOpponents(gameState)
        # enemy_position = [gameState.getAgentState(enemy).getPosition() for enemy in enemyIndices if gameState.getAgentState(enemy).getPosition()!=None]
        # if len(enemy_position) > 0:
        #     for pos in enemy_position:
        #         distance_to_enemy1.append(self.getMazeDistance(myposition,pos))
        # if len(distance_to_enemy1) > 0:
        #     min_distance1 = min(distance_to_enemy1)
        # else:
        #     min_distance1 = 0
        # if len(enemy_position) > 0:
        #     for pos in enemy_position:
        #         distance_to_enemy2.append(self.getMazeDistance(newPosition,pos))
        # if len(distance_to_enemy2) > 0:
        #     min_distance2 = min(distance_to_enemy2)
        # else:
        #     min_distance2 = 0
        # if min_distance2 < min_distance1:
        #     reward -= 5
        # else:
        #     reward += 5
        # reward for the home:
        # distance_to_home1 = []
        # distance_to_home2 = []
        # if len(self.homeboundary) > 0:
        #     for home in self.homeboundary:
        #         distance_to_home1.append(self.getMazeDistance(myposition, home))
        # if len(distance_to_home1) > 0:
        #     min_distance1 = min(distance_to_home1)
        # else:
        #     min_distance1 = 0
        # if len(self.homeboundary) > 0:
        #     for home in self.homeboundary:
        #         distance_to_home2.append(self.getMazeDistance(newPosition,home))
        # if len(distance_to_home2) > 0:
        #     min_distance2 = min(distance_to_home2)
        # else:
        #     min_distance2 = 0
        #
        # if min_distance2 < min_distance1:
        #     reward += 1
        # else:
        #     reward -= 1
        # reward for the capsules:
        # distance_to_capsule1 = []
        # distance_to_capsule2 = []
        # if len(capsuleslist) > 0:
        #     for capsule in capsuleslist:
        #         distance_to_capsule1.append(self.getMazeDistance(myposition, capsule))
        # if len(distance_to_capsule1) > 0:
        #     min_distance1 = min(distance_to_capsule1)
        # else:
        #     min_distance1 = 0
        # if len(capsuleslist) > 0:
        #     for capsule in capsuleslist:
        #         distance_to_capsule2.append(self.getMazeDistance(newPosition,capsule))
        # if len(distance_to_capsule2) > 0:
        #     min_distance2 = min(distance_to_capsule2)
        # else:
        #     min_distance2 = 0
        #
        # if min_distance2 < min_distance1:
        #     reward += 5
        # else:
        #     reward -= 5


        return reward

    def update(self, gameState, bestAction, successor, reward):
        difference = reward + self.discount * self.getMaxvalue(successor) - self.getQValue(gameState, bestAction)
        weights = self.getWeights()
        if len(weights) == 0:
            weights[(gameState, bestAction)] = 0
        features = self.getFeatures(gameState, bestAction)
        for key in features.keys():
            weights[key] += features[key] * self.alpha * difference
        self.weightsO = weights.copy()
        print(self.weightsO)

# defenders
class Defender(DummyAgent):
    # def get_home_pos(self, gameState):
    #     homeboundary = []
    #     mid_x = int(self.get_middle_line() - 1)
    #     for i in range(self.height):
    #         if not gameState.hasWall(mid_x, i):
    #             homeboundary.append((mid_x,i))
    #     return homeboundary

    def registerInitialState(self, gameState):
        DummyAgent.registerInitialState(self, gameState)
        # self.homeboundary = self.get_home_pos(gameState)
        self.defense_point = self.homeboundary[int(len(self.homeboundary) / 2)]

    def get_scaredtime(self, gameState):
        enemies = self.getTeam(gameState)
        value = 0
        for enemy in enemies:
            scaredTimer = gameState.getAgentState(enemy).scaredTimer
            if scaredTimer > 0:
                value = scaredTimer
            if scaredTimer == 0 and gameState.getAgentState(enemy).getPosition() != None:
                return 0
        return value

    def chooseAction(self, gameState):
        """
        Picks among actions randomly.
        """
        flag1 = flag2 = False
        if self.red:
            score = gameState.getScore()
        else:
            score = -1 * gameState.getScore()
        be_eaten = False
        my_pos = gameState.getAgentPosition(self.index)
        enemies = [gameState.getAgentState(i) for i in self.getOpponents(gameState)]
        enemy_Pos = []
        distoenemy = float("inf")
        for enemy in enemies:
            if enemy.isPacman and enemy.getPosition() is not None:
                enemy_Pos.append(enemy)
                if distoenemy > self.getMazeDistance(my_pos, enemy.getPosition()):
                    distoenemy = self.getMazeDistance(my_pos, enemy.getPosition())
        if my_pos == self.targetPosition:
            self.targetPosition = None
        if len(enemy_Pos) > 0:
            positions = [agent.getPosition() for agent in enemy_Pos]
            self.targetPosition = min(positions, key=lambda x: self.getMazeDistance(my_pos, x))
        if self.lastObservedFood:
            eaten = set(self.lastObservedFood) - set(self.getFoodYouAreDefending(gameState).asList())
            if len(eaten) > 0:
                positions = [position for position in eaten]
                min_dis = float('inf')
                for position in positions:
                    for food in self.getFoodYouAreDefending(gameState).asList() + self.getCapsulesYouAreDefending(
                            gameState):
                        distance = self.getMazeDistance(position, food)
                        if distance < min_dis:
                            min_dis = distance
                            self.targetPosition = food
                be_eaten = True
        else:
            self.targetPosition = self.defense_point
        self.lastObservedFood = self.getFoodYouAreDefending(gameState).asList()

        if self.targetPosition is None:
            if len(self.getFoodYouAreDefending(gameState).asList()) <= 5:
                food = self.getFoodYouAreDefending(gameState).asList() + self.getCapsulesYouAreDefending(gameState)
                if not food:
                    self.targetPosition = self.defense_point
                else:
                    self.targetPosition = random.choice(food)
            else:
                self.targetPosition = self.defense_point
        if self.get_scaredtime(gameState) > 5 or (100 < gameState.data.timeleft < 300 and score <= 0):
            flag1 = True
            self.targetPosition = None
        if (self.get_scaredtime(gameState) <= 5 or gameState.data.timeleft <= 100) and gameState.getAgentState(
                self.index).isPacman and gameState.getAgentState(self.index).numCarrying > 0:
            flag2 = True
            self.targetPosition = None

        '''
        You should change this in your own agent.
        '''

        if not flag1 and not flag2:
            actions = gameState.getLegalActions(self.index)
            actions.remove('Stop')
            select_actions = []
            values = []
            # print(actions)
            for action in actions:
                # print(actions)
                new_state = gameState.generateSuccessor(self.index, action)
                last_state = self.getPreviousObservation()
                if not new_state.getAgentState(self.index).isPacman:
                    new_pos = new_state.getAgentPosition(self.index)
                    # print("1111", be_eaten)
                    if be_eaten:
                        last_pos = last_state.getAgentPosition(self.index)
                        if last_pos != new_pos:
                            # print("2222")
                            select_actions.append(action)
                            values.append(self.getMazeDistance(new_pos, self.targetPosition))
                        else:
                            # print("3333")
                            reverse = action
                    else:
                        # print("444444")
                        select_actions.append(action)
                        values.append(self.getMazeDistance(new_pos, self.targetPosition))
                else:
                    select_actions.append("Stop")
                    values.append(self.getMazeDistance(my_pos, self.targetPosition))
            if len(select_actions) == 0:
                select_actions.append(reverse)
                values.append(0)

            best_cost = min(values)
            new_action = list(filter(lambda x: x[0] == best_cost, zip(values, select_actions)))
            return random.choice(new_action)[1]
        elif flag1 and not flag2:
            problem = FoodSearchProblem(gameState, self, self.index)

            # if my_pos[0] < self.mid_x and self.get_scaredtime(gameState) > 5:
            #     heuristic = self.SuperHeuristic
            # else:
            #     heuristic = self.GhostHeuristic
            actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            if actions is None and not gameState.getAgentState(self.index).isPacman:
                return "Stop"
            elif actions is None and gameState.getAgentState(self.index).isPacman:
                problem = BackHomeProblem(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            return actions[0]
        else:
            problem = BackHomeProblem(gameState, self, self.index)
            actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            if actions == [] and not gameState.getAgentState(self.index).isPacman:
                return "Stop"
            if actions == [] and gameState.getAgentState(self.index).isPacman:
                problem = BackHomeProblem(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.nullHeuristic)
            return actions[0]
# class for problems
class PositionSearchProblem:
    def __init__(self, gameState, agent, agentIndex=0, costFn=lambda x: 1):
        self.walls = gameState.getWalls()
        self.costFn = costFn
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE

    def getStartState(self):
        return self.startState

    def isGoalState(self, state):

        util.raiseNotDefined()

    def getSuccessors(self, state):
        successors = []
        for action in [Directions.NORTH, Directions.SOUTH, Directions.EAST, Directions.WEST]:
            x, y = state
            dx, dy = game.Actions.directionToVector(action)
            nextx, nexty = int(x + dx), int(y + dy)
            if not self.walls[nextx][nexty]:
                nextState = (nextx, nexty)
                cost = self.costFn(nextState)
                successors.append((nextState, action, cost))

        self._expanded += 1  # DO NOT CHANGE
        if state not in self._visited:
            self._visited[state] = True
            self._visitedlist.append(state)

        return successors

    def getCostOfActions(self, actions):
        if actions == None: return 999999
        x, y = self.getStartState()
        cost = 0
        for action in actions:
            dx, dy = game.Actions.directionToVector(action)
            x, y = int(x + dx), int(y + dy)
            if self.walls[x][y]: return 999999
            cost += self.costFn((x, y))
        return cost

class DenfenderFoodProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex=0, foodlist=[]):
        self.food = foodlist
        self.capsule = agent.getCapsules(gameState)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE

    def isGoalState(self, state):
        return state in self.food

class FoodSearchProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex=0):
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE

    def isGoalState(self, state):
        return state in self.food.asList()


class BackHomeProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex=0):
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE
        self.homeBoundary = agent.get_home_pos(gameState)

    def getStartState(self):
        return self.startState

    def isGoalState(self, state):
        # the goal state is the boudary of home or the positon of capsule
        return state in self.homeBoundary


