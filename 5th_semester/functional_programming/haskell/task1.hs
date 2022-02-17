type Point = (Float, Float, Float)
dist :: Point -> Point -> Float
dist (x1, y1, z1) (x2, y2, z2) = sqrt((x2 - x1)^2+(y2 - y1)^2+(z2 - z1)^2)
v :: Point -> Point -> Float -> Float
v a b r = pi * (dist b a) * r^2 / 3

main = print(v (0, 0, 0) (0, 0, 4) 3)
