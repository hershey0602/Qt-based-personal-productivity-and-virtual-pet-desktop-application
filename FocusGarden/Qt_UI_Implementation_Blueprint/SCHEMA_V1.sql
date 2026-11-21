-- FG-API-1.0 schema blueprint; no production database is delivered.
PRAGMA foreign_keys=ON;
CREATE TABLE IF NOT EXISTS tasks (
 id INTEGER PRIMARY KEY AUTOINCREMENT,
 title TEXT NOT NULL CHECK(length(trim(title)) BETWEEN 1 AND 100),
 note TEXT NOT NULL DEFAULT '' CHECK(length(note)<=500),
 category INTEGER NOT NULL DEFAULT 0 CHECK(category BETWEEN 0 AND 2),
 priority INTEGER NOT NULL DEFAULT 1 CHECK(priority BETWEEN 0 AND 2),
 due_at_utc TEXT,
 created_at_utc TEXT NOT NULL,
 completed INTEGER NOT NULL DEFAULT 0 CHECK(completed IN(0,1)),
 deleted INTEGER NOT NULL DEFAULT 0 CHECK(deleted IN(0,1))
);
CREATE INDEX IF NOT EXISTS ix_tasks_filter_due ON tasks(deleted,completed,category,priority,due_at_utc,created_at_utc);
CREATE TABLE IF NOT EXISTS sessions (
 session_id TEXT PRIMARY KEY NOT NULL,
 task_id INTEGER REFERENCES tasks(id) ON DELETE SET NULL,
 task_title_snapshot TEXT NOT NULL,
 mode INTEGER NOT NULL CHECK(mode IN(0,1)),
 planned_seconds INTEGER NOT NULL CHECK(planned_seconds BETWEEN 900 AND 5400 AND planned_seconds%300=0),
 elapsed_seconds INTEGER NOT NULL DEFAULT 0 CHECK(elapsed_seconds BETWEEN 0 AND planned_seconds),
 started_at_utc TEXT NOT NULL,
 ended_at_utc TEXT,
 ended_local_date TEXT,
 updated_at_utc TEXT NOT NULL,
 status INTEGER NOT NULL CHECK(status BETWEEN 0 AND 3),
 reward_state INTEGER NOT NULL CHECK(reward_state BETWEEN 0 AND 2),
 CHECK((status IN(0,1) AND ended_at_utc IS NULL AND ended_local_date IS NULL AND reward_state=0)
    OR (status=2 AND ended_at_utc IS NOT NULL AND ended_local_date IS NOT NULL AND reward_state IN(1,2) AND elapsed_seconds=planned_seconds)
    OR (status=3 AND ended_at_utc IS NOT NULL AND ended_local_date IS NOT NULL AND reward_state=0))
);
CREATE INDEX IF NOT EXISTS ix_sessions_date ON sessions(ended_local_date,status,ended_at_utc);
CREATE INDEX IF NOT EXISTS ix_pending_rewards ON sessions(status,reward_state);
CREATE TABLE IF NOT EXISTS pet_state (
 singleton_id INTEGER PRIMARY KEY CHECK(singleton_id=1),
 cumulative_exp INTEGER NOT NULL DEFAULT 0 CHECK(cumulative_exp>=0),
 mood INTEGER NOT NULL DEFAULT 70 CHECK(mood BETWEEN 0 AND 100)
);
INSERT OR IGNORE INTO pet_state(singleton_id,cumulative_exp,mood) VALUES(1,0,70);
CREATE TABLE IF NOT EXISTS coin_ledger (
 id INTEGER PRIMARY KEY AUTOINCREMENT,
 operation_id TEXT NOT NULL UNIQUE,
 session_id TEXT REFERENCES sessions(session_id),
 reason TEXT NOT NULL CHECK(reason IN('focus_reward','feed')),
 delta INTEGER NOT NULL,
 created_at_utc TEXT NOT NULL,
 exp_before INTEGER,
 exp_after INTEGER,
 mood_before INTEGER,
 mood_after INTEGER,
 CHECK((reason='focus_reward' AND session_id IS NOT NULL AND delta>0 AND exp_before IS NULL AND exp_after IS NULL AND mood_before IS NULL AND mood_after IS NULL)
 OR (reason='feed' AND session_id IS NULL AND delta=-15 AND exp_before IS NOT NULL AND exp_after IS NOT NULL AND mood_before IS NOT NULL AND mood_after IS NOT NULL AND exp_after=exp_before+10 AND mood_before BETWEEN 0 AND 100 AND mood_after=min(100,mood_before+15)))
);
CREATE UNIQUE INDEX IF NOT EXISTS ux_focus_reward ON coin_ledger(session_id) WHERE reason='focus_reward';
PRAGMA user_version=1;
