
drop table door_state;

create table door_state (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  rec_time text not null,
  state int not null,
  temperature text not null
);

insert into door_state (rec_time, state, temperature)
values
 ('2020-07-17 13:10:00',1,30.5),
 ('2020-07-17 13:12:00',2,30.5),
 ('2020-07-17 13:30:00',1,30.5),
 ('2020-07-17 13:32:00',2,30.5),
 ('2020-07-17 13:50:00',1,30.5),
 ('2020-07-17 14:00:00',2,30.5),
 ('2020-07-17 14:10:00',1,30.5),
 ('2020-07-17 14:20:00',2,30.5);


select *
from door_state
order by id desc;

drop table readings;

create table readings (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  timestamp text not null,
  temperature text not null,
  temperature_units text not null,
  humidity text not null,
  humidity_units text not null
);

insert into readings (timestamp, temperature, temperature_units, humidity, humidity_units)
values
 ('2021-02-05 13:10:00','60.0','degF','30.0','%'),
 ('2021-02-05 13:10:02','60.1','degF','30.1','%'),
 ('2021-02-05 13:10:04','60.2','degF','30.2','%'),
 ('2021-02-05 13:10:06','60.3','degF','30.3','%'),
 ('2021-02-05 13:10:08','60.4','degF','30.4','%'),
 ('2021-02-05 13:10:10','60.5','degF','30.5','%'),
 ('2021-02-05 13:10:12','60.6','degF','30.6','%'),
 ('2021-02-05 13:10:14','60.7','degF','30.7','%');

select *
from readings
order by id desc;
