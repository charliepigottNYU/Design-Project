#!/bin/bash
export PGPASSWORD=CS4523
psql -U postgres -d postgres -a -f ../../sql/setup.sql 
