import asyncio
from asyncio import sleep
import asyncpg

import json

MAX_LINE = 10000

def try_to_get_boolean_value(json_line, key):
    try:
        result = json_line[key]
    except KeyError:
        result = False
    return bool(result)

def try_to_get_guid_str_value(json_line, key):
    try:
        result = json_line[key]
    except KeyError:
        result = "0"*32
    return result

def try_to_get_date_str_value(json_line, key):
    try:
        result = json_line[key]
    except KeyError:
        result = "0"*4
    return result

def try_to_get_str_value(json_line, key):
    try:
        result = json_line[key]
    except KeyError:
        result = str(None)
    return result

def try_to_get_int_value(json_line, key):
    try:
        result = json_line[key]
    except KeyError:
        result = -1
    return int(result)

class Users:
    QUERY = """INSERT INTO users VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, TO_TIMESTAMP($12, 'YYYY-MM-DDTHH24:MI:ss'), TO_TIMESTAMP($13, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/Users.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            Users.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'AccountId'),
            try_to_get_int_value(line, 'Reputation'),
            try_to_get_int_value(line, 'Views'),
            try_to_get_int_value(line, 'DownVotes'),
            try_to_get_int_value(line, 'UpVotes'),
            try_to_get_str_value(line, 'DisplayName'),
            try_to_get_str_value(line, 'Location'),
            try_to_get_str_value(line, 'ProfileImageUrl'),
            try_to_get_str_value(line, 'WebsiteUrl'),
            try_to_get_str_value(line, 'AboutMe'),
            try_to_get_date_str_value(line, 'CreationDate'),
            try_to_get_date_str_value(line, 'LastAccessDate'),
        )
        await sleep(.1)

class Posts:
    QUERY = """INSERT INTO posts VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, TO_TIMESTAMP($18, 'YYYY-MM-DDTHH24:MI:ss'), TO_TIMESTAMP($19, 'YYYY-MM-DDTHH24:MI:ss'), TO_TIMESTAMP($20, 'YYYY-MM-DDTHH24:MI:ss'), TO_TIMESTAMP($21, 'YYYY-MM-DDTHH24:MI:ss'), TO_TIMESTAMP($22, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/Posts.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            Posts.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'OwnerUserId'),
            try_to_get_int_value(line, 'LastEditorUserId'),
            try_to_get_int_value(line, 'PostTypeId'),
            try_to_get_int_value(line, 'AcceptedAnswerId'),
            try_to_get_int_value(line, 'Score'),
            try_to_get_int_value(line, 'ParentId'),
            try_to_get_int_value(line, 'ViewCount'),
            try_to_get_int_value(line, 'AnswerCount'),
            try_to_get_int_value(line, 'CommentCount'),
            try_to_get_str_value(line, 'OwnerDisplayName'),
            try_to_get_str_value(line, 'LastEditorDisplayName'),
            try_to_get_str_value(line, 'Title'),
            try_to_get_str_value(line, 'Tags'),
            try_to_get_str_value(line, 'ContentLicense'),
            try_to_get_str_value(line, 'Body'),
            try_to_get_int_value(line, 'FavoriteCount'),
            try_to_get_date_str_value(line, 'CreationDate'),
            try_to_get_date_str_value(line, 'CommunityOwnedDate'),
            try_to_get_date_str_value(line, 'ClosedDate'),
            try_to_get_date_str_value(line, 'LastEditDate'),
            try_to_get_date_str_value(line, 'LastActivityDate'),
        )
        await sleep(.1)

class PostLinks:
    QUERY = """INSERT INTO post_links VALUES ($1, $2, $3, $4, TO_TIMESTAMP($5, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/PostLinks.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            PostLinks.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'RelatedPostId'),
            try_to_get_int_value(line, 'PostId'),
            try_to_get_int_value(line, 'LinkTypeId'),
            try_to_get_date_str_value(line, 'CreationDate'),
        )
        await sleep(.1)

class PostHistory:
    QUERY = """INSERT INTO post_history VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, TO_TIMESTAMP($10, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/PostHistory.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            PostHistory.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'PostId'),
            try_to_get_int_value(line, 'UserId'),
            try_to_get_int_value(line, 'PostHistoryTypeId'),
            try_to_get_str_value(line, 'UserDisplayName'),
            try_to_get_str_value(line, 'ContentLicense'),
            try_to_get_guid_str_value(line, 'RevisionGuid'),
            try_to_get_str_value(line, 'Text'),
            try_to_get_str_value(line, 'Comment'),
            try_to_get_date_str_value(line, 'CreationDate'),
        )
        await sleep(.1)

class Comments:
    QUERY = """INSERT INTO comments VALUES ($1, $2, $3, $4, $5, $6, $7, TO_TIMESTAMP($8, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/Comments.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            Comments.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'PostId'),
            try_to_get_int_value(line, 'UserId'),
            try_to_get_int_value(line, 'Score'),
            try_to_get_str_value(line, 'ContentLicense'),
            try_to_get_str_value(line, 'UserDisplayName'),
            try_to_get_str_value(line, 'Text'),
            try_to_get_date_str_value(line, 'CreationDate'),
        )
        await sleep(.1)

class Votes:
    QUERY = """INSERT INTO votes VALUES ($1, $2, $3, $4, $5, TO_TIMESTAMP($6, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/Votes.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            Votes.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'UserId'),
            try_to_get_int_value(line, 'PostId'),
            try_to_get_int_value(line, 'VoteTypeId'),
            try_to_get_int_value(line, 'BountyAmount'),
            try_to_get_date_str_value(line, 'CreationDate'),
        )
        await sleep(.1)

class Badges:
    QUERY = """INSERT INTO badges VALUES ($1, $2, $3, $4, $5, TO_TIMESTAMP($6, 'YYYY-MM-DDTHH24:MI:ss'))"""
    FILE_NAME = "json/Badges.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            Badges.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'UserId'),
            try_to_get_int_value(line, 'Class'),
            try_to_get_str_value(line, 'Name'),
            try_to_get_boolean_value(line, 'TagBased'),
            try_to_get_date_str_value(line, 'CreationDate'),
        )
        await sleep(.1)

class Tags:
    QUERY = """INSERT INTO tags VALUES ($1, $2, $3, $4, $5)"""
    FILE_NAME = "json/Tags.json"

    async def make_request(db_pool, line):
        await db_pool.fetch(
            Tags.QUERY,
            try_to_get_int_value(line, 'Id'),
            try_to_get_int_value(line, 'ExcerptPostId'),
            try_to_get_int_value(line, 'WikiPostId'),
            try_to_get_str_value(line, 'TagName'),
            try_to_get_int_value(line, 'Count'),
        )
        await sleep(.1)

ClassName = Tags

async def main():
    with open(ClassName.FILE_NAME, "r") as fh:
        data = json.load(fh) # загружаем структуру из файла
    
    len_data = len(data)
    
    chunk = 200
    tasks = []
    pended = 0
    
    db_pool = await asyncpg.create_pool("postgresql://127.0.0.1:5432/postgres")
    
    for line in data:
        tasks.append(asyncio.create_task(ClassName.make_request(db_pool, line)))
        pended += 1
        if len(tasks) == chunk or pended == len_data:
            await asyncio.gather(*tasks)
            tasks = []
            print(pended)
        if pended == MAX_LINE:
            break

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
main()

