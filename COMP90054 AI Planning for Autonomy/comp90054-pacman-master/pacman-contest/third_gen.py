# third_gen.py
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
               first = 'Defender', second = 'AstarAgent'):
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

# offenders
class AstarAgent(CaptureAgent):
    def registerInitialState(self, gameState):
        CaptureAgent.registerInitialState(self, gameState)
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
        em = ExploreMap(self, gameState)
        self.foodlist = em.getFoodlst(gameState)
        self.safe_food = em.get_safe_food(self.foodlist)
        self.dangerous_food = [ food for food in self.food if food not in self.safe_food]



    def chooseAction(self, gameState):
        next_safe_food = []
        next_danger_food = []
        all_food_list =self.getFood(gameState).asList()
        hasEnemies = False
        myposition = gameState.getAgentState(self.index).getPosition()
        teamposition = None
        # udpate the safe food list and the dangerous food list
        for food in all_food_list:
            if food in self.safe_food:
                next_safe_food.append(food)
            elif food in self.dangerous_food:
                next_danger_food.append(food)
        self.safe_food = next_safe_food
        self.dangerous_food = next_danger_food
        # get teammate position
        for mem in self.teammatesIndices:
            if mem is not self.index:
                teamposition = gameState.getAgentState(mem).getPosition()
        # get enemies positions
        for enemy in self.enemiesIndices:
            enemy_pos = gameState.getAgentPosition(enemy)
            if enemy_pos:
                dis_to_me = self.getMazeDistance(enemy_pos,myposition)
                dis_to_teammate = self.getMazeDistance(enemy_pos, teamposition)
                if dis_to_me < dis_to_teammate:
                    hasEnemies = True
        # if there are safe food and agent does not have any pacs
        if gameState.getAgentState(self.index).numCarrying < 1 and (len(self.safe_food) > 0):
            problem = SafeFoodProblem(gameState, self, self.index)
            actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            return actions[0]

        # if len(self.safe_food) > 0 and not hasEnemies:
        #     dis_to_food = 99999
        #     for food in self.safe_food:
        #         dis_temp = self.getMazeDistance(myposition, food)
        #         if dis_temp < dis_to_food:
        #             dis_to_food = dis_temp
        #     problem = SafeFoodProblem(gameState, self, self.index)
        #     return self.aStar_search(problem, gameState, self.GhostHeuristic)[0]

        # if there are no safe food and but there is one Capsule
        if len(self.safe_food) <= 0 and len(self.dangerous_food) > 0 and len(self.getCapsules(gameState))!=0:
            problem = SearchCapsule(gameState, self, self.index)
            actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            return actions[0]

        # if agent takes capsule
        if self.opponents_scaredtime(gameState) != None:
            if self.opponents_scaredtime(gameState) > 20 and len(self.dangerous_food) > 0:
                problem =DangerousFoodProblem(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.nullHeuristic)
                return actions[0]
            # elif self.opponents_scaredtime(gameState)<10:
            #     problem =EatGhostProblem(gameState, self, self.index)
            #     actions = self.aStar_search(problem, gameState, self.nullHeuristic)
        #
        # if self.dis_to_ghost(gameState) != None and self.dis_to_ghost(gameState)[0]< 6 and \
        # self.dis_to_ghost(gameState)[1].scaredTimer < 5:
        #     problem = BackHomeProblem(gameState, self, self.index)
        #     if len(self.aStar_search(problem, self.GhostHeuristic)) == 0:
        #         return 'Stop'
        #     else:
        #         return self.aStar_search(problem, gameState, self.GhostHeuristic)[0]
        # if agent does not take three points, but meet enemy
        if gameState.getAgentState(self.index).numCarrying >= 1 and hasEnemies:
            if len(self.getCapsules(gameState)) != 0:
                problem = SearchCapsule(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            else:
                problem = BackHomeProblem(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            return actions[0]

        # if our agent takes at least three points, then go home
        if gameState.getAgentState(self.index).numCarrying > 3:
            if self.opponents_scaredtime(gameState) > 10:
                problem = FoodSearchProblem(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            else:
                problem = BackHomeProblem(gameState, self, self.index)
                actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
            return actions[0]

        problem  = FoodSearchProblem(gameState, self, self.index)
        actions = self.aStar_search(problem, gameState, self.GhostHeuristic)
        return actions[0]
    """
    some useful functions
    """
    def get_legal_position(self, gameState):
        # This function is to return the legal positions
        return [pos for pos in gameState.getWalls().asList(False) if pos[1] > 1]

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

    def opponents_scaredtime(self, gameState):
        enemies = self.getOpponents(gameState)
        for enemy in enemies:
            scaredTimer = gameState.getAgentState(enemy).scaredTimer
            if scaredTimer >0:
                return scaredTimer
        return 0

    def dis_to_ghost(self, gameState):
        myposition = gameState.getAgentState(self.index).getPosition()
        enemies = [gameState.getAgentState(enemy) for enemy in self.getOpponents(gameState)]
        ghosts = [g for g in enemies if g.isPacman and g.getPosition()!=None]
        ghostState = None
        if len(ghosts) > 0:
            distance = 99999
            for g in ghosts:
                dis_temp = self.getMazeDistance(myposition, g.getPosition())
                if dis_temp < distance:
                    distance = dis_temp
                    ghostState = g
            return [distance, ghostState]
        else:
            return None
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
            if distance <= 1:
                heuristic = distance + 99999
        return heuristic


    """
    algorithm
    """
    def aStar_search(self, problem, gameState, heuristic = nullHeuristic):
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
                if problem.isGoalState(currentNode):
                    return currentPath
                successors = problem.getSuccessors(currentNode)
                for expandNode, action, stepCost in successors:
                    newPath = currentPath + [action]
                    priority = len(newPath) + heuristic(expandNode, gameState)
                    openedlst.push((expandNode, newPath), priority)
                    if expandNode not in closedlst:
                        best_g[expandNode] = len(newPath)
        return None
# defenders
class Defender(CaptureAgent):
    def registerInitialState(self, gameState):
        CaptureAgent.registerInitialState(self, gameState)
        self.distancer.getMazeDistances()

        self.lastObservedFood = None
        self.probDict = {}
        self.mapwidth = gameState.data.layout.width
        self.mapheight = gameState.data.layout.height
        self.midX = 0.0
        if self.red:
            self.midX = (self.mapwidth - 2) / 2
        else:
            self.midX = ((self.mapwidth - 2) / 2) + 1
        self.midY = self.mapheight / 2
        self.noWall = []
        for i in range(0, self.mapheight):
            if not gameState.hasWall(int(self.midX), i):
                self.noWall.append((int(self.midX), i))
        self.defense_point = self.noWall[int(len(self.noWall) / 2)]
        self.targetPosition = None

    def opponents_scaredtime(self, gameState):
        enemies = self.getTeam(gameState)
        for enemy in enemies:
            scaredTimer = gameState.getAgentState(enemy).scaredTimer
            if scaredTimer > 0:
                return scaredTimer
        return 0

    def chooseAction(self, gameState):
        """
        Picks among actions randomly.
        """

        be_eaten = False
        my_pos = gameState.getAgentPosition(self.index)
        enemies = [gameState.getAgentState(i) for i in self.getOpponents(gameState)]
        enemy_Pos = []
        distoenemy = float("inf")
        for enemy in enemies:
            if enemy.isPacman and enemy.getPosition() != None:
                enemy_Pos.append(enemy)
                if distoenemy > self.getMazeDistance(my_pos, enemy.getPosition()):
                    distoenemy = self.getMazeDistance(my_pos, enemy.getPosition())
            # else:
            #     self.targetPosition = self.defense_point
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
                    for food in self.getFoodYouAreDefending(gameState).asList() + self.getCapsulesYouAreDefending(gameState):
                        distance = self.getMazeDistance(position, food)
                        if distance < min_dis:
                            min_dis = distance
                            self.targetPosition = food
                be_eaten = True
        self.lastObservedFood = self.getFoodYouAreDefending(gameState).asList()
        if self.targetPosition == None:
            if len(self.getFoodYouAreDefending(gameState).asList()) <= 5:
                food = self.getFoodYouAreDefending(gameState).asList() + self.getCapsulesYouAreDefending(gameState)
                if food == []:
                    self.targetPosition = self.defense_point
                else:
                    self.targetPosition = random.choice(food)
            else:
                self.targetPosition = self.defense_point
        if self.opponents_scaredtime(gameState) > 15:
            self.targetPosition = self.defense_point
        '''
        You should change this in your own agent.
        '''
        actions = gameState.getLegalActions(self.index)
        actions.remove('Stop')
        select_actions = []
        values = []
        for action in actions:
            new_state = gameState.generateSuccessor(self.index, action)
            last_state = self.getPreviousObservation()
            if not new_state.getAgentState(self.index).isPacman:
                new_pos = new_state.getAgentPosition(self.index)
                if be_eaten:

                    last_pos = last_state.getAgentPosition(self.index)
                    if last_pos != new_pos:
                        select_actions.append(action)
                        values.append(self.getMazeDistance(new_pos, self.targetPosition))
                    else:
                        reverse = action
                else:
                    select_actions.append(action)
                    values.append(self.getMazeDistance(new_pos, self.targetPosition))
        if len(select_actions) == 0:
            select_actions.append(reverse)
            values.append(0)

        best_cost = min(values)
        new_action = list(filter(lambda x: x[0] == best_cost, zip(values, select_actions)))
        return random.choice(new_action)[1]

# class for problems
class PositionSearchProblem:
    def __init__(self, gameState, agent, agentIndex = 0,costFn = lambda x: 1):
        self.walls = gameState.getWalls()
        self.costFn = costFn
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self._visited, self._visitedlist, self._expanded = {}, [], 0 # DO NOT CHANGE

    def getStartState(self):
      return self.startState

    def isGoalState(self, state):

      util.raiseNotDefined()

    def getSuccessors(self, state):
        successors = []
        for action in [Directions.NORTH, Directions.SOUTH, Directions.EAST, Directions.WEST]:
            x,y = state
            dx, dy = game.Actions.directionToVector(action)
            nextx, nexty = int(x + dx), int(y + dy)
            if not self.walls[nextx][nexty]:
                nextState = (nextx, nexty)
                cost = self.costFn(nextState)
                successors.append( ( nextState, action, cost) )

        self._expanded += 1 # DO NOT CHANGE
        if state not in self._visited:
            self._visited[state] = True
            self._visitedlist.append(state)

        return successors

    def getCostOfActions(self, actions):
        if actions == None: return 999999
        x,y= self.getStartState()
        cost = 0
        for action in actions:
            dx, dy = game.Actions.directionToVector(action)
            x, y = int(x + dx), int(y + dy)
            if self.walls[x][y]: return 999999
            cost += self.costFn((x,y))
        return cost


class SafeFoodProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex = 0):
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE
        self.carry = gameState.getAgentState(agentIndex).numCarrying
        self.foodLeft = len(self.food.asList())
        self.safe_food = agent.safe_food

    def isGoalState(self, state):
        return state in self.safe_food


class DangerousFoodProblem(PositionSearchProblem):
     def __init__(self, gameState, agent, agentIndex = 0):
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        # Store info for the PositionSearchProblem (no need to change this)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE
        self.carry = gameState.getAgentState(agentIndex).numCarrying
        self.foodLeft = len(self.food.asList())
        self.dangerous_food = agent.dangerous_food

     def isGoalState(self, state):
         return state in self.dangerous_food

class FoodSearchProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex = 0):
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE
    def isGoalState(self,state):
        return state in self.food.asList()

class BackHomeProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex = 0):
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
        return state in self.homeBoundary or state in self.capsule

class SearchCapsule(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex = 0):
        # Store the food for later reference
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        # Store info for the PositionSearchProblem (no need to change this)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE


    def isGoalState(self, state):
        # the goal state is the location of capsule
        return state in self.capsule
class EatGhostProblem(PositionSearchProblem):
    def __init__(self, gameState, agent, agentIndex = 0):
         # Store the food for later reference
        self.food = agent.getFood(gameState)
        self.capsule = agent.getCapsules(gameState)
        # Store info for the PositionSearchProblem (no need to change this)
        self.startState = gameState.getAgentState(agentIndex).getPosition()
        self.walls = gameState.getWalls()
        self.costFn = lambda x: 1
        self._visited, self._visitedlist, self._expanded = {}, [], 0  # DO NOT CHANGE
        self.near_ghost = self.getNearestGhost(agent, gameState)

    def getNearestGhost(self,agent,gameState):
        enemiesIndices = agent.getOpponents(gameState)
        enemies =[gameState.getAgentState(enemy) for enemy in enemiesIndices]
        ghosts = [enemy.getPosition() for enemy in enemies if enemy.isPacman and enemy.getPosition() != None]
        distance = 99999
        result = {}
        if len(ghosts) > 0 :
            for ghost in ghosts:
                print("starState:", self.startState)
                print("Ghost:", ghost)
                dis_temp = agent.getMazeDistance(self.startState, ghost)
                if dis_temp < distance:
                    distance = dis_temp
                    result[distance] = ghost
            return result[distance]
        else:
            return None
    def isGoalState(self, state):
        if self.near_ghost != None:
            return state in self.near_ghost
        else:
            return False


class ExploreMap:
    '''
    This is a class for exploring the whole map to find the food that we can safely eat
    '''
    def __init__(self, agent, gameState):
        # store the food that our agent needs to eat
        self.food =agent.getFood(gameState).asList()
        self.walls = gameState.getWalls()
        self.width = gameState.data.layout.width
        self.height = gameState.data.layout.height
        self.legal_positions = self.get_legal_position(gameState)
        self.mid_x = self.get_middle_line(agent)
        self.homeboundary =agent.homeboundary

    def get_middle_line(self,agent):
        if agent.red:
            mid_x = (self.width - 2) / 2

        else:
            mid_x = (self.width - 2) / 2 + 1
        return mid_x

    def get_legal_position(self, gameState):
      return [pos for pos in gameState.getWalls().asList(False) if pos[1] > 1]

    # def get_home_pos(self,agent):
    #     home_domain = []
    #     for pos in self.legal_positions:
    #         if agent.red and pos[0] <= self.mid_x:
    #             home_domain.append(pos)
    #         elif not agent.red and pos >= self.mid_x:
    #             home_domain.append(pos)
    #     return home_domain

    def getFoodlst(self, gameState):
        foodlist = []
        for food in self.food:
            nearPoints = []
            possiblePoints = []
            count = 0
            nearPoints.append((food[0] + 1, food[1]))
            nearPoints.append((food[0] - 1, food[1]))
            nearPoints.append((food[0], food[1] + 1))
            nearPoints.append((food[0], food[1] - 1))
            for np in nearPoints:
                if np not in self.walls.asList():
                    count += 1
                    possiblePoints.append(np)
            if count > 1:
                foodlist.append((food, possiblePoints))
        return foodlist

    def get_safe_food(self, foodlist):
        safe_food_list = []
        for elem in foodlist:
            count = self.get_valid_safe_food(elem)
            if count > 1:
                safe_food_list.append(elem[0])
        return safe_food_list


    def isGoalState(self, state):
        return state in self.homeboundary

    def getSuccessors(self, state):
        successors = []
        for action in [Directions.NORTH, Directions.SOUTH, Directions.EAST, Directions.WEST]:
            x, y = state
            dx, dy = game.Actions.directionToVector(action)
            nextx, nexty = int(x + dx), int(y + dy)
            if not self.walls[nextx][nexty]:
                nextState = (nextx, nexty)
                successors.append((nextState, action))
        return successors

    def get_valid_safe_food(self, foodlist):
        food = foodlist[0]
        possiblePoints = foodlist[1]
        visited = []
        visited.append(food)
        count = 0
        for pos in possiblePoints:
            closed = copy.deepcopy(visited)
            if self.DFS(pos, closed):
                count = count + 1
        return count

    def DFS(self, possiblePoint, closed):
        from util import Stack
        openedlst = Stack()
        openedlst.push((possiblePoint, []))
        while not openedlst.isEmpty():
            currentNode, actions = openedlst.pop()
            if currentNode not in closed:
                closed.append(currentNode)
                if self.isGoalState(currentNode):
                    return True
                expandings = self.getSuccessors(currentNode)
                for expandNode, action in expandings:
                    if expandNode not in closed:
                        openedlst.push((expandNode,actions + [action]))










