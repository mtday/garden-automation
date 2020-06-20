
DROP TABLE IF EXISTS temperatures;
DROP TABLE IF EXISTS humidities;
DROP TABLE IF EXISTS pressures;
DROP TABLE IF EXISTS lights;
DROP TABLE IF EXISTS tank_volumes;

CREATE TABLE IF NOT EXISTS temperatures (
    id             SERIAL       NOT NULL,
    device         VARCHAR(32)  NOT NULL,
    timestamp      TIMESTAMP    NOT NULL DEFAULT NOW(),
    temperature    FLOAT        NOT NULL,

    CONSTRAINT temperatures_pk PRIMARY KEY (id)
);

CREATE INDEX IF NOT EXISTS idx_temperatures_device ON temperatures (device);
CREATE INDEX IF NOT EXISTS idx_temperatures_timestamp ON temperatures (timestamp);


CREATE TABLE IF NOT EXISTS humidities (
    id             SERIAL       NOT NULL,
    device         VARCHAR(32)  NOT NULL,
    timestamp      TIMESTAMP    NOT NULL DEFAULT NOW(),
    humidity       FLOAT        NOT NULL,

    CONSTRAINT humidities_pk PRIMARY KEY (id)
);

CREATE INDEX IF NOT EXISTS idx_humidities_device ON humidities (device);
CREATE INDEX IF NOT EXISTS idx_humidities_timestamp ON humidities (timestamp);


CREATE TABLE IF NOT EXISTS pressures (
    id             SERIAL       NOT NULL,
    device         VARCHAR(32)  NOT NULL,
    timestamp      TIMESTAMP    NOT NULL DEFAULT NOW(),
    pressure       FLOAT        NOT NULL,

    CONSTRAINT pressures_pk PRIMARY KEY (id)
);

CREATE INDEX IF NOT EXISTS idx_pressures_device ON pressures (device);
CREATE INDEX IF NOT EXISTS idx_pressures_timestamp ON pressures (timestamp);


CREATE TABLE IF NOT EXISTS lights (
    id             SERIAL       NOT NULL,
    device         VARCHAR(32)  NOT NULL,
    timestamp      TIMESTAMP    NOT NULL DEFAULT NOW(),
    light          FLOAT        NOT NULL,

    CONSTRAINT lights_pk PRIMARY KEY (id)
);

CREATE INDEX IF NOT EXISTS idx_lights_device ON lights (device);
CREATE INDEX IF NOT EXISTS idx_lights_timestamp ON lights (timestamp);


CREATE TABLE IF NOT EXISTS tank_volumes (
    id             SERIAL       NOT NULL,
    device         VARCHAR(32)  NOT NULL,
    timestamp      TIMESTAMP    NOT NULL DEFAULT NOW(),
    tank           INTEGER      NOT NULL,
    volume         FLOAT        NOT NULL,

    CONSTRAINT tank_volumes_pk PRIMARY KEY (id)
);

CREATE INDEX IF NOT EXISTS idx_tank_volumes_device ON tank_volumes (device);
CREATE INDEX IF NOT EXISTS idx_tank_volumes_timestamp ON tank_volumes (timestamp);
CREATE INDEX IF NOT EXISTS idx_tank_volumes_tank ON tank_volumes (tank);

