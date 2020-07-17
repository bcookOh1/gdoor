
drop table door_state;

create table door_state (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  rec_time text not null,
  state int not null
);

insert into door_state (rec_time, state)
values
 ('2020-07-17 13:10:00',1),
 ('2020-07-17 13:12:00',2),
 ('2020-07-17 13:30:00',1),
 ('2020-07-17 13:32:00',2),
 ('2020-07-17 13:50:00',1),
 ('2020-07-17 14:00:00',2),
 ('2020-07-17 14:10:00',1),
 ('2020-07-17 14:20:00',2);

select *
from door_state
order by id desc;
