findNames::Char->[(String,String)]->[String] 
findNames b list = foldr (\(name,surname) acc -> ((head(name):('.':surname)):acc)) [] (filter (\(name,surname) -> head(surname)/=b) list) 

main = print (findNames 'B' [("Adam", "Smith"),("Sue", "Brown"),("Tom", "Taylor")] )
