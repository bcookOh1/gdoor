
drop table door_state;

create table door_state(
  start text primary key,
  start_state int not null,
  end text not null,
  end_state int not null
);

insert into door_state 
values
 ('2020-07-01 13:10:00',1,'2020-7-01 13:10:30',2),
 ('2020-07-01 13:12:00',2,'2020-7-01 13:12:30',1),
 ('2020-07-01 13:30:00',1,'2020-7-01 13:30:30',2),
 ('2020-07-01 13:32:00',2,'2020-7-01 13:32:30',1),
 ('2020-07-01 13:50:00',1,'2020-7-01 13:50:30',2),
 ('2020-07-01 14:00:00',2,'2020-7-01 14:00:30',1),
 ('2020-07-01 14:10:00',1,'2020-7-01 14:10:30',2),
 ('2020-07-01 14:20:00',2,'2020-7-01 13:40:30',1);

select *
from door_state
order by start desc;
