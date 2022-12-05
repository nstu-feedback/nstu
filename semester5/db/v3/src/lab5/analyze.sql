EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number = 'a555aa';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number > 'a555aa';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number < 'a555aa';

-- BTREE INDEX ANALYZE
DROP INDEX IF EXISTS car_number_idk;

CREATE INDEX car_number_idx ON "car"(number);

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number = 'a555aa';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number > 'a555aa';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number < 'a555aa';


-- HASH INDEX ANALYZE
DROP INDEX IF EXISTS car_number_idx;

CREATE INDEX car_number_idx ON "car" USING HASH (number);

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number = 'a555aa';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number > 'a555aa';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE number < 'a555aa';

-- UPPER ANALYZE
DROP INDEX IF EXISTS car_number_idx;

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE UPPER(number) = 'A555AA';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE UPPER(number) > 'A555AA';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE UPPER(number) < 'A555AA';

-- UPPER INDEX ANALYZE
CREATE INDEX car_number_idx ON "car"(UPPER(number));

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE UPPER(number) = 'A555AA';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE UPPER(number) > 'A555AA';

EXPLAIN ANALYZE
    SELECT * FROM "car" WHERE UPPER(number) < 'A555AA';

