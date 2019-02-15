explain SELECT sname from student
    where sid in (
        select sid from cs_relation where 
        cid=3 and score>all(
            select avg(score) as avg from cs_relation group by cid
        )
    );

explain SELECT sname from student
    where sid in (
        select sid from cs_relation where 
        cid=2 and score>(
            select avg(score) as avg from cs_relation where cid=3
        )
    );

create index q1_index on cs_relation (cid,score);
drop index q1_index on cs_relation;

//////////////////////////////////////////////////////

create VIEW q2_view AS
    select cname from course join cs_relation using (cid) where sid=1;

/////////////////////////////////////////////////////

delimiter #

create TRIGGER q3_insert before insert on cs_relation
for each row
begin 
if new.score < 0 then set new.score=0;
elseif new.score>100 then set new.score=100;
end if;
END#

create TRIGGER q3_update before update on cs_relation
for each row
begin 
if new.score < 0 then set new.score=0;
elseif new.score>100 then set new.score=100;
end if;
END#

delimiter ;

insert into cs_relation values(3,2,1002,5);

update cs_relation set score=-122 where sid=3 and cid=2;

delete from cs_relation where cid=2 and sid=3;

/////////////////////////////////////////////////////

alter table cs_relation
add COLUMN attendance int;

update cs_relation set attendance=3*cid+4*sid where cid=1 and sid=2;


-------------------------------------------------------------

explain
select sname from student where sid in (
select distinct sid from cs_relation as A where
(select count(distinct cid) from cs_relation as B where 
B.sid=A.sid or B.sid=2) = (select count(cid) from cs_relation as C where C.sid=A.sid)
);

explain
select sname from 
student join 
(
select distinct sid from cs_relation as A where
(select count(distinct cid) from cs_relation as B where 
B.sid=A.sid or B.sid=1) = (select count(cid) from cs_relation as C where C.sid=A.sid)
) as p using (sid);

--(cid,sid) already primary key

-- create index q5_index on cs_relation(sid,cid);

create index q5_index on student(sname);

--------------------------------------------------------------

-- create VIEW q5_view as
-- select sname from 
-- student natural join 
-- (
-- select distinct sid from cs_relation as A where
-- (select count(distinct cid) from cs_relation as B where 
-- B.sid=A.sid or B.sid=1) = (select count(cid) from cs_relation as C where C.sid=A.sid)
-- ) as p where substring(sname,1,5)=substring((select sname from student where sid=1),1,5)
-- ;

-- create index q6_index on q5_view(sid,cid,sname);

ALTER table student 
add COLUMN fname VARCHAR(5);

update student set fname=substring(sname,1,5);

explain
select sname from 
student natural join 
(
select distinct sid from cs_relation as A where
(select count(distinct cid) from cs_relation as B where 
B.sid=A.sid or B.sid=1) = (select count(cid) from cs_relation as C where C.sid=A.sid)
) as p where fname = (select fname from student where sid=1)
;

create index q6index on student(fname);


-------------------------------------------------------

-- select "F" as grade,cid,sid from st

create view q7_view as
(select cid,sid,"F" as grade from cs_relation where score<40) 
union
(select cid,sid,"B" as grade from cs_relation where score>=40 and score<70) 
union 
(select cid,sid,"A" as grade from cs_relation where score>40);




----------------------------------------------
insert into q7_view values(4,2,"A");



-- create view q8_view as 
-- select sid , cid , 
-- CASE 
-- WHEN score >= 70 THEN 'A'
-- WHEN score >= 40 AND score < 70 THEN 'B'
-- ELSE 'F'
-- END AS GRADE 
-- from cs_relation;

alter table cs_relation add column grade varchar(2);

create view q8_view as
select cid,sid,grade from cs_relation;


----------------------------------------------


delimiter #

CREATE TRIGGER q9_trigger before insert on cs_relation
for each row
BEGIN
if new.score < 40 then set new.grade ="F";
elseif new.score >=40 and new.score<70 then set new.grade="B";
ELSE set new.grade="A";
end if;
end#

delimiter ;


delimiter #

CREATE TRIGGER q9_trigger_upd before update on cs_relation
for each row
BEGIN
if new.score < 40 then set new.grade ="F";
elseif new.score >=40 and new.score<70 then set new.grade="B";
ELSE set new.grade="A";
end if;
end#

delimiter ;