from captureAgents import CaptureAgent
from baselineTeam import ReflexCaptureAgent
import random, time, util
from game import Directions
from util import nearestPoint
import copy


def createTeam(firstIndex, secondIndex, isRed,
               first='Defender', second='AStarAgent'):
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
        if len(enemy_Pos) > 0:
            positions = [agent.getPosition() for agent in enemy_Pos]
            self.targetPosition = min(positions, key=lambda x: self.getMazeDistance(my_pos, x))
        if self.lastObservedFood:
            eaten = set(self.lastObservedFood) - set(self.getFoodYouAreDefending(gameState).asList())
            if len(eaten) > 0:
                positions = [position for position in eaten]
                min_dis = float('inf')
                for position in positions:
                    for food in self.getFoodYouAreDefending(gameState).asList():
                        distance = self.getMazeDistance(position, food)
                        if distance < min_dis:
                            min_dis = distance
                            self.targetPosition = food

                be_eaten = True
        else:
            self.targetPosition = self.defense_point
        self.lastObservedFood = self.getFoodYouAreDefending(gameState).asList()

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

class AStarAgent(ReflexCaptureAgent):
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
    A distanceCalculator instance caches the  (i)  Your interface to the pacman world:
          Pacman is a complex environment.  You probably don't want to
          read through all of the code we wrote to make the game runs
          correctly.  This section contains the parts of the code
          that you will need to understand in order to complete the
          project.  There is also some code in game.py that you should
          understand.

  (ii)  The hidden secrets of pacman:
          This section contains all of the logic code that the pacman
          environment uses to decide who can move where, who dies when
          things collide, etc.  You shouldn't need to read this section
          of code, but you can if you want.

  (iii) Framework to start a game:
          The final section contains the code for reading the command
          you use to set up the game, then starting up a new game, along with
          linking in all the external parts (agent functions, graphics).
          Check this section out to see all the options available to you. maze distances
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

        self.walls = gameState.getWalls()
        self.width = self.walls.width
        self.height = self.walls.height
        self.hasFood = 0
        self.lastAction = None
        self.lastState = None
        self.capsules = self.getCapsules(gameState)
        self.isSuper = False
        # print self.capsules
        # record whether the pacman is going back home
        self.isGoingHome = False
        # record whether the pacman is getting the capsule
        self.isGettingCapsule = False

        # Index of enemies and team
        self.enemyIndex = self.getOpponents(gameState)
        self.teamIndex = self.getTeam(gameState)

    def chooseAction(self, gameState):
        # Check the situation
        if not self.isGettingCapsule and not self.isGoingHome:
            hasEnemy = False

            for enemy in self.enemyIndex:

                enemyLoc = gameState.getAgentPosition(enemy)
                if not enemyLoc == None:
                    distanceToMe = self.getMazeDistance(enemyLoc, gameState.getAgentPosition(self.index))

                    teammate = copy.copy(self.teamIndex)
                    teammate.remove(self.index)
                    distanceToTeam = self.getMazeDistance(enemyLoc, gameState.getAgentPosition(teammate[0]))

                    if not distanceToMe > distanceToTeam:
                        hasEnemy = True
            if hasEnemy and self.hasFood > 0:
                self.isGoingHome = True

        if not self.lastState == None:
            if gameState.getAgentPosition(self.index) in self.getFood(self.lastState).asList():
                self.hasFood += 1
            elif self.getMazeDistance(gameState.getAgentPosition(self.index),
                                      self.lastState.getAgentPosition(self.index)) > 5:
                self.isGoingHome = False
                self.hasFood = 0
            else:
                if self.atHome(gameState.getAgentPosition(self.index)):
                    if self.hasFood > 0:
                        self.hasFood = 0
                    self.isGoingHome = False

        if gameState.getAgentPosition(self.index) in self.capsules:
            self.isGettingCapsule = False
            self.isGettingHome = False
            self.isSuper = True
            self.capsules.remove(gameState.getAgentPosition(self.index))

        if self.isSuper:
            self.isGoingHome = False
            goOn = True
            for enemy in self.enemyIndex:
                if gameState.getAgentState(self.enemyIndex[0]).scaredTimer < 8:
                    goOn = False
            if not goOn:
                self.isSuper = False
                self.isGoingHome = True

        action = self.aStarSearch(gameState)[0]
        self.lastAction = action
        self.lastState = gameState

        return action

    def getCost(self, gamestate, location, action):
        if self.red:
            if location[0] < self.width / 2 - 1:
                return 0
        else:
            if location[0] > self.width / 2:
                return 0
        next_location = self.move(location, action)
        cost = 0
        for enemy in self.enemyIndex:
            if not gamestate.getAgentPosition(enemy) == None:
                enemyLoc = gamestate.getAgentPosition(enemy)
                if self.atHome(location):
                    difference = 0
                else:
                    difference = max(
                        self.getMazeDistance(location, enemyLoc) - self.getMazeDistance(next_location, enemyLoc), 0)
                distance = self.getMazeDistance(next_location, enemyLoc)
                if distance == 0:
                    cost += difference * 200
                else:
                    cost += difference * 200 / distance

        return cost

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

    def getFoodHeuristic(self, location, foodGrid):
        food_location = foodGrid.asList()
        if not len(food_location) == 0:
            food_1 = food_location[0]
            food_2 = food_location[0]
            max_distance = 0
            for current_food in food_location:
                for select_food in food_location:
                    distance = self.getMazeDistance(current_food, select_food)
                    if max_distance < distance:
                        max_distance = distance
                        food_1 = current_food
                        food_2 = select_food
            d1 = self.getMazeDistance(location, food_1)
            d2 = self.getMazeDistance(location, food_2)
            return max_distance + min(d1, d2)
        else:
            return 0

    # Huristic function that is used when the Agent is geting home
    def getHomeHeuristic(self, location):
        if self.red:
            midX = int(self.width / 2 - 1)
        else:
            midX = int(self.width / 2)

        yaxis = list(range(0, self.height))

        ywall = []
        # check for walls and record them
        for y in yaxis:
            if self.walls[midX][y]:
                ywall.append(y)
        # remove walls from yaxis
        for y in ywall:
            yaxis.remove(y)

        minDistance = 1000
        for y in yaxis:
            distance = self.getMazeDistance(location, (midX, y))
            if distance < minDistance:
                minDistance = distance

        return minDistance

    def atHome(self, location):
        if self.red:
            if location[0] < self.width / 2:
                return True
            else:
                return False
        else:
            if location[0] > self.width / 2 - 1:
                return True
            else:
                return False

    def getHeuristic(self, location, foods):
        if self.isGoingHome:
            return self.getHomeHeuristic(location)
        else:
            return self.getFoodHeuristic(location, foods)

    def aStarSearch(self, gameState):
        """Search the node that has the lowest combined cost and heuristic first."""
        "*** YOUR CODE HERE ***"
        actions = ['North', 'South', 'West', 'East']
        currentLoc = gameState.getAgentPosition(self.index)
        foods = self.getFood(gameState)
        open = util.PriorityQueue()
        """use visited dictionary to re-open"""
        visited = dict()

        n1 = SearchNode(currentLoc, self.getHeuristic(currentLoc, foods), 0, [], None)
        open.push(n1, n1.cost)
        visited[currentLoc] = 0
        while True:
            if open.isEmpty():
                # print "aStar error", gameState.getLegalActions(self.index)
                return [random.choice(gameState.getLegalActions(self.index))]

            node = open.pop()
            # print node.state
            # print "check node", node.state
            if (node.state in foods.asList() and not self.isGoingHome) or (
                    self.red and node.state[0] < self.width / 2 and currentLoc[
                0] > self.width / 2 - 1 and self.hasFood > 0 and self.isGoingHome) or (
                    not self.red and node.state[0] > self.width / 2 - 1 and currentLoc[
                0] < self.width / 2 and self.hasFood > 0 and self.isGoingHome) or (
                    node.state in self.capsules):
                return node.actions
            else:
                for action in actions:
                    nextLoc = self.move(node.state, action)
                    x, y = nextLoc
                    if self.walls[x][y]:
                        continue
                    cost = self.getCost(gameState, node.state, action)
                    successor = (nextLoc, action, 0.5)
                    origin_cost = visited.get(successor[0])
                    new_cost = node.cost + successor[2] + cost
                    if origin_cost is None or origin_cost > new_cost:
                        action_list = copy.copy(node.actions)
                        action_list.append(successor[1])
                        f_value = self.getHeuristic(nextLoc, foods) + new_cost
                        succ_node = SearchNode(successor[0], f_value, new_cost, action_list, node)
                        open.push(succ_node, f_value)
                        visited[successor[0]] = new_cost

    def move(self, location, action):
        x, y = location
        if action == 'North':
            return (int(x), int(y) + 1)
        elif action == 'South':
            return (int(x), int(y) - 1)
        elif action == 'West':
            return (int(x) - 1, int(y))
        elif action == 'East':
            return (int(x) + 1, int(y))
        else:
            return (int(x), int(y))


class SearchNode:
    def __init__(self, state, f_value, cost, actions, parent):
        self.state = state
        self.f_value = f_value
        self.cost = cost
        self.actions = actions
        self.parent = parent
