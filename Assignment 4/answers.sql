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