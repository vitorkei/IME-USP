#!/bin/bash

mysql -u root -p < create_schema.sql
mysql -u root -p < populate_database.sql

