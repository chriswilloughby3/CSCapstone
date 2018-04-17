'''
Created on Jan 15, 2018

@author: christopherwilloughby
'''

import json
from bson import json_util
from bson import Binary, Code
from bson.json_util import dumps
from datetime import datetime
from pymongo import MongoClient
import pymongo
from objc._objc import NULL

connection = MongoClient('localhost', 27017)
db = connection['city']
collection = db['inspections']

def createDocument(document, searchData = NULL):
    if (searchData != NULL):
        document = {document : searchData}
    try:
        collection.insert_one(document)
    except:
        result = False
    else:
        result = True
    return result

def readDocument(document, searchData = NULL):
    if (searchData != NULL):
        document = {document: searchData}
    try:
        result = dumps(collection.find_one(document))     
    except Exception, e:
        result = str(e)               
    return result

def updateDocument(document, dataValues, searchData = NULL):
    if (searchData != NULL):
        document = {document: searchData}
    try:
        result = collection.update_one(document, {"$set" : dataValues})
        result = dumps(str(result))
    except Exception, e:
        result = str(e)
    return result

def deleteDocument(document, searchData = NULL):
    if (searchData != NULL):
        document = {document: searchData}
    try:
        result = collection.delete_one(document)
        result = dumps(str(result))
    except Exception, e:
        result = str(e)
    return result

def main():
    document = {"id" : "20032-2017-ACME",
                "certificate_number" : 9998888,
                "business_name" : "ACME Explosives",
                "date" : datetime.utcnow(),
                "result" : "Business Padlocked",
                "sector": "Explosive Retail Dealer - 999",
                "address" : { 
                    "number" : 1721,
                    "street" : "Boom Road",
                    "city" : "BRONX",
                    "zip" : 10463}}
    
    search = {"id" : "20032-2017-ACME"}
    
    updatedData = {"business_name" : "ACME Explosives Inc",
                    "address.city" : "Detroit",
                    "address.state" : "MI",
                    "address.zip" : "48204"}
    
    print(createDocument(document))
    print(createDocument("id", "Bob's Stuff"))
    print(readDocument(search))
    print(readDocument("id", "Bob's Stuff"))
    print(updateDocument(search, updatedData))
    print(readDocument(search))
    print(updateDocument("id", updatedData, "Bob's Stuff"))
    print(readDocument("id", "Bob's Stuff"))
    print(deleteDocument(search))
    print(readDocument(search))
    print(deleteDocument("id", "Bob's Stuff"))
    print(readDocument("id", "Bob's Stuff"))
    
main()
