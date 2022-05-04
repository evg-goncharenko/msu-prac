-- Database module

{-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE FlexibleContexts   #-}
{-# LANGUAGE DerivingStrategies #-}
{-# OPTIONS_GHC -Wno-incomplete-patterns #-}

module DBase where

import Database.HDBC.Sqlite3 (connectSqlite3, Connection())
import Database.HDBC

type Name = String
type Result = Int
type ID = Int

-- Сreate user, if it doesn't exist, return id
createNewUser:: Name -> Connection -> IO (Int)
createNewUser name conn = do
  selectPreparetion <- prepare conn "SELECT id from users where name = ?"
  _ <- execute selectPreparetion [toSql name] 
  results <- fetchAllRowsAL selectPreparetion
  if null results then do
    _ <- run conn "INSERT INTO users (name) VALUES (?)" [toSql name]
    commit conn
    newUserId <- (createNewUser name conn)
    return newUserId
  else do
    return (getIDFromResult results)
        
-- Type conversion id to int
getIDFromResult:: [[(String, SqlValue)]] -> Int
getIDFromResult mas = fromSql (snd(head(head mas)))

getConnection:: IO Connection
getConnection = do
    conn <- connectSqlite3 "dbTetris"
    _ <- run conn "CREATE TABLE if not exists users (id integer PRIMARY KEY AUTOINCREMENT NOT NULL, name varchar(27) NOT NULL)" []
    _ <- run conn "CREATE TABLE if not exists records (id integer NOT NULL, result int NOT NULL)" []
    commit conn
    return conn

someFunc:: IO()
someFunc = do
    conn <- getConnection
    s <- createNewUser "alexs" conn 
    putStrLn (show s)
    d <- getTop10 conn
    disconnect conn
    putStrLn (show d)

-- Write result to the records table
writeResultToTable:: Result -> ID -> Connection -> IO ()
writeResultToTable result idBD conn = do
    _ <- run conn "INSERT INTO records (id, result) VALUES (?, ?)" [toSql idBD, toSql result]
    commit conn

-- Parsing array returned by the table
getListFromQuery::[[(String, SqlValue)]]->[(String, Int)]
getListFromQuery mas = map (\elemInMas -> (fromSql( snd (head elemInMas) ), fromSql( snd ( head ( tail elemInMas ) ) ) ) ) mas

-- Get top 10 results from records table
getTop10:: Connection -> IO [(String, Int)]
getTop10 conn = do 
    selectPreparetion <- prepare conn "SELECT name, MAX(result) from records left join users on users.id = records.id group by name ORDER BY result DESC limit 10"
    _ <- execute selectPreparetion []
    results <- fetchAllRowsAL' selectPreparetion
    return (getListFromQuery results)
