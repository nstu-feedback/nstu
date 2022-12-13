DROP DATABASE IF EXISTS v3;
DROP ROLE IF EXISTS me;

CREATE USER
    me
WITH
    LOGIN
    SUPERUSER
    CREATEDB
    CREATEROLE;

CREATE DATABASE
    v3
OWNER
    me;
