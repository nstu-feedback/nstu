CREATE OR REPLACE FUNCTION city_part_of_total(_city_id INTEGER)
    RETURNS DOUBLE PRECISION AS $$
DECLARE
    qty_total   INTEGER;
    qty_by_city INTEGER;
BEGIN
    SELECT COUNT(*) INTO qty_total FROM "product";
    IF qty_total = 0 THEN
        RETURN 0;
    END IF;

    SELECT COUNT(*) INTO qty_by_city FROM "product" WHERE city_id = _city_id;
    RETURN qty_by_city::real / qty_total::real;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION city_part_of_total(city_name VARCHAR)
    RETURNS REAL AS $$
DECLARE
    city_id INTEGER;
BEGIN
    SELECT id INTO city_id FROM "city" WHERE name = city_name;
    IF NOT FOUND THEN
        RAISE EXCEPTION 'city % not found', city_name;
    END IF;
    RETURN (SELECT city_part_of_total(city_id));
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION provider_part_of_total(_provider_id INTEGER)
    RETURNS REAL AS $$
DECLARE
    qty_total       INTEGER;
    qty_by_provider INTEGER;
BEGIN
    SELECT COUNT(*) INTO qty_total FROM "product";
    IF qty_total = 0 THEN
        RETURN 0;
    END IF;

    SELECT COUNT(*) INTO qty_by_provider FROM "product"
        WHERE provider_id = _provider_id;
    RETURN qty_by_provider::real / qty_total::real;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION provider_part_of_total(provider_name VARCHAR)
    RETURNS REAL AS $$
DECLARE
    provider_id INTEGER;
BEGIN
     SELECT id INTO provider_id FROM "provider" WHERE name = provider_name;
     IF NOT FOUND THEN
         RAISE EXCEPTION 'provider % not found', provider_name;
     END IF;
     RETURN (SELECT provider_part_of_total(provider_id));
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION sale_period_part_of_total(
    date_from TIMESTAMP,
    date_to   TIMESTAMP
) RETURNS REAL AS $$
DECLARE
    qty_total          INTEGER;
    qty_by_sale_period INTEGER;
BEGIN
    IF date_from >= date_to THEN
        RAISE EXCEPTION 'invalid period: from % to %', date_from, date_to;
    end if;

    SELECT SUM(qty) INTO qty_total FROM "sales";
    IF qty_total = 0 THEN
        RETURN 0;
    END IF;

    SELECT SUM(qty) INTO qty_by_sale_period FROM "sales"
        WHERE date BETWEEN date_from AND date_to;
    RETURN qty_by_sale_period::real / qty_total::real;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION cheap_part_of_total(
    _price       REAL,
    _customer_id INTEGER
) RETURNS TABLE (
    by_customer REAL,
    total       REAL
) AS $$
DECLARE
    qty_total       INTEGER;
    qty_cheap       INTEGER;
    qty_by_customer INTEGER;
    _by_customer    REAL;
    _total          REAL;
BEGIN
    IF _price < 0 THEN
        RAISE EXCEPTION 'price cannot be less than 0';
    END IF;

    IF _customer_id <= 0 THEN
        RAISE EXCEPTION 'customer_id must be greater than 0';
    END IF;

    SELECT COUNT(*) INTO qty_total FROM "product";
    SELECT COUNT(*) INTO qty_cheap FROM "product" WHERE price < _price;
    SELECT qty_cheap::real / qty_total::real INTO _total;

    SELECT COUNT(*) INTO qty_by_customer FROM "product"
        WHERE price < _price AND id IN (
            SELECT DISTINCT product_id FROM "sales"
            WHERE customer_id = _customer_id);
    SELECT qty_by_customer::real / qty_total::real INTO _by_customer;

    RETURN QUERY (SELECT _by_customer, _total);
END;
$$ LANGUAGE 'plpgsql';
