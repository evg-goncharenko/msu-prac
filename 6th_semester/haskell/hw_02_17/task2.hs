import Data.Char

capitalize :: [String] -> [String]
capitalize = map(\x -> capitalizeWord x)
capitalizeWord :: String -> String
capitalizeWord "" = ""
capitalizeWord x = toUpper (head x) : tail x

main = print(capitalize ["education", "is", "the", "movement", "from", "darkness", "to", "light"])
