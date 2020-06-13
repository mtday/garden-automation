
-- CREATE DATABASE garden;
-- CREATE USER 'garden'@'localhost' IDENTIFIED BY 'garden';
-- GRANT ALL PRIVILEGES ON garden.* TO 'garden'@'localhost';

-- mysql -u garden -p garden < db.sql

DROP TABLE IF EXISTS temperatures;

CREATE TABLE IF NOT EXISTS temperatures (
    `id`             INTEGER      NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `device`         VARCHAR(32)  NOT NULL,
    `location`       VARCHAR(32)  NOT NULL,
    `timestamp`      TIMESTAMP    NOT NULL DEFAULT NOW(),
    `temperature`    FLOAT        NOT NULL,

    INDEX idx_temperatures_device USING HASH (`device`),
    INDEX idx_temperatures_location USING HASH (`location`),
    INDEX idx_temperatures_timestamp USING HASH (`timestamp`)
);

