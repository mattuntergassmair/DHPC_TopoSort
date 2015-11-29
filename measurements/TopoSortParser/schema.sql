BEGIN TRANSACTION;
CREATE TABLE `timings` (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`thread_id`	INTEGER,
	`name`	TEXT,
	`value`	REAL
);
CREATE TABLE "threads" (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`measurement_id`	INTEGER,
	`thread_id`	INTEGER,
	`processed_nodes`	INTEGER
);
CREATE TABLE "measurements" (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`date`	INTEGER,
	`number_of_threads`	INTEGER,
	`processors`	INTEGER,
	`comment`	NUMERIC,
	`total_time`	REAL,
	`algorithm`	TEXT,
	`graph_type`	TEXT,
	`graph_num_nodes`	INTEGER,
	`graph_num_edges`	INTEGER
);
COMMIT;
