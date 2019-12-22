# search.py
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


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE IF YOU WANT TO PRACTICE ***"
    stack = util.Stack()
    visited = []
    actions = []
    costs = 0

    stack.push((problem.getStartState(), actions, costs))

    while not stack.isEmpty():
        (cur_state,actions,costs) = stack.pop()

        if problem.isGoalState(cur_state):
            return actions

        if cur_state not in visited:
            visited.append(cur_state)

            for successor in problem.getSuccessors(cur_state):
                if successor[0] not in visited:
                    new_actions = actions + [successor[1]]
                    new_cost = costs + successor[2]
                    stack.push((successor[0], new_actions, new_cost))
    # if fail, return false
    util.raiseNotDefined()


def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE IF YOU WANT TO PRACTICE ***"

    queue = util.Queue()
    visited = []
    actions = []
    costs = 0

    queue.push((problem.getStartState(), actions, costs))

    while not queue.isEmpty():
        (cur_state, actions, costs) = queue.pop()

        if problem.isGoalState(cur_state):
            return actions

        if cur_state not in visited:
            visited.append(cur_state)

            for successor in problem.getSuccessors(cur_state):
                if successor[0] not in visited:
                    new_actions = actions + [successor[1]]
                    new_cost = costs + successor[2]
                    queue.push((successor[0], new_actions, new_cost))
    # if fail, return false
    util.raiseNotDefined()


def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE IF YOU WANT TO PRACTICE ***"

    pqueue = util.PriorityQueue()
    visited = []
    actions = []
    costs = 0

    pqueue.push((problem.getStartState(), actions, costs),0)

    while not pqueue.isEmpty():
        (cur_state, actions, costs) = pqueue.pop()

        if problem.isGoalState(cur_state):
            return actions

        if cur_state not in visited:
            visited.append(cur_state)

            for successor in problem.getSuccessors(cur_state):
                if successor[0] not in visited:
                    new_actions = actions + [successor[1]]
                    new_cost = problem.getCostOfActions(new_actions)
                    pqueue.push((successor[0], new_actions, new_cost),new_cost)
    # if fail, return false
    util.raiseNotDefined()

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE IF YOU WANT TO PRACTICE ***"

    # from the pseudocode from the slides
    pqueue = util.PriorityQueue()
    visited = []
    actions = []
    costs = 0
    pqueue.push((problem.getStartState(), actions), costs)

    while not pqueue.isEmpty():
        (cur_state, actions) = pqueue.pop()

        if cur_state not in visited:
            visited.append(cur_state)
            if problem.isGoalState(cur_state):
                return actions
            for successors in problem.getSuccessors(cur_state):
                (state,step,cost) = successors
                new_action = actions + [step]
                new_cost = problem.getCostOfActions(new_action) + heuristic(state,problem)
                if state not in visited:
                    pqueue.push((state,new_action),new_cost)

    util.raiseNotDefined()

def iterativeDeepeningSearch(problem):
    """Search the deepest node in an iterative manner."""
    "*** YOUR CODE HERE FOR TASK 1 ***"
    stack = util.Stack()
    limit = -1

    while True:
        limit += 1
        visited = []
        stack.push((problem.getStartState(), [], 0))
        (cur_state, actions, costs) = stack.pop()
        visited.append(cur_state)

        while not problem.isGoalState(cur_state):
                for successor in problem.getSuccessors(cur_state):
                    if problem.isGoalState(successor[0]):
                        return actions + [successor[1]]
                    if (successor[0] not in visited) and (costs + 1 <= limit):
                        new_state = successor[0]
                        new_actions = actions + [successor[1]]
                        new_cost = costs + 1
                        stack.push((new_state, new_actions, new_cost))
                        visited.append(successor[0])

                if stack.isEmpty():
                    break

                (cur_state, actions, costs) = stack.pop()
                visited.append(cur_state)

        if problem.isGoalState(cur_state):
            return actions


def waStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has has the weighted (x 2) lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE FOR TASK 2 ***"

    # from the pseudocode from the slides
    w = 2
    pqueue = util.PriorityQueue()
    visited = []
    actions = []
    costs = 0
    pqueue.push((problem.getStartState(), actions), costs)

    while not pqueue.isEmpty():
        (cur_state, actions) = pqueue.pop()

        if cur_state not in visited:
            visited.append(cur_state)
            if problem.isGoalState(cur_state):
                return actions
            for successors in problem.getSuccessors(cur_state):
                (state, step, cost) = successors
                new_action = actions + [step]
                new_cost = problem.getCostOfActions(new_action) + w * heuristic(state, problem)
                if state not in visited:
                    pqueue.push((state, new_action), new_cost)

    util.raiseNotDefined()

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
ids = iterativeDeepeningSearch
wastar = waStarSearch
