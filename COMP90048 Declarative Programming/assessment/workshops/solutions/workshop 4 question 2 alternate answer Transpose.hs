transpose :: [[x]] -> [[x]]
transpose array = 
          let nempty [] = False
              nempty _  = True
          in
          if and (map nempty array) then
              map head array : transpose (map tail array)
          else []
