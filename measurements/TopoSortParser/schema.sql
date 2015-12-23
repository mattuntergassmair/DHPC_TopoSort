BEGIN TRANSACTION;
CREATE TABLE `timings` (	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,	`thread_id`	INTEGER,	`name`	TEXT,	`value`	REAL);
CREATE TABLE "threads" (	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,	`measurement_id`	INTEGER,	`thread_id`	INTEGER,	`processed_nodes`	INTEGER, `processed_edges`	INTEGER);
CREATE TABLE "measurements" (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`date`	BLOB,
	`number_of_threads`	BLOB,
	`processors`	TEXT,
	`comment`	NUMERIC,
	`total_time`	REAL,
	`algorithm`	TEXT,
	`graph_type`	TEXT,
	`graph_num_nodes`	INTEGER,
	`graph_num_edges`	INTEGER,
	`optimistic`	INTEGER,
	`enable_analysis`	INTEGER,
	`verbose`	INTEGER,
	`debug`	INTEGER,
	`hostname`	TEXT,
	`error_code`	INTEGER,
	`graph_depth`	INTEGER,
	`graph_density`	INTEGER
);
COMMIT;
