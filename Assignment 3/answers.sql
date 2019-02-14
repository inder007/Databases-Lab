select sname from (student)  join (select sid from cs_relation where cid not in (select cid from cs_relation where sid=2))as c using (sid);
-- OR
select c.sid from cs_relation c where not exists (select * from cs_relation s where s.sid=2 and c.cid=s.cid);

////////////////////////////////////

select s.sname from student s join (select c3.sid from student c3 where not exists (select c.cid from (select c2.cid from cs_relation c2 where c2.sid=3) as c where not exists (select * from cs_relation c1 where c1.sid=c3.sid and c1.cid=c.cid))) as p using (sid);
-- OR
select * from student s where (select count(sc.sid)  from cs_relation sc join cs_relation sc1 using (cid) where sc1.sid=3 && sc.sid!=sc1.sid && s.sid=sc.sid)=(select count(sid) from cs_relation where sid=3);
-- OR
select distinct sid from course_stud as A where (select count(distinct cid) from course_stud as B where B.sid=A.sid or B.sid=2)=(select count(cid) from course_stud as C where C.sid=A.sid);

/////////////////////////////////

select distinct sname from student join (select s.sid from cs_relation s where s.cid in (select c1.cid from lc_relation c1 where c1.lid=1 or c1.lid=2)) as p using (sid);
-- OR 
select s1.sid from cs_relation s1 where s1.cid in (select s.cid from course s where exists (select * from lc_relation l where (l.lid=1 or l.lid=2) and l.cid=s.cid) );

/////////////////////////////////

select s3.sid from student s3 where (s3.sid in (select s.sid from cs_relation s where s.cid in (select s1.cid from lc_relation s1 where s1.lid=1)) and s3.sid not in (select s.sid from cs_relation s where s.cid in (select s1.cid from lc_relation s1 where s1.lid=2))) or (s3.sid in (select s.sid from cs_relation s where s.cid not in (select s1.cid from lc_relation s1 where s1.lid=1)) and s3.sid in (select s.sid from cs_relation s where s.cid in (select s1.cid from lc_relation s1 where s1.lid=2)));
select * from student join (select * from cs_relation where (cid in (select c1.cid from lc_relation c1 where c1.lid=1) and cid not in (select c2.cid from lc_relation c2 where c2.lid=2)) or (cid in (select c3.cid from lc_relation c3 where c3.lid=2) and cid not in (select c4.cid from lc_relation c4 where c4.lid=1))) as aa using (sid);

///////////////////////////////

select sid from cs_relation where score>(select avg(s.sid) from cs_relation s where s.cid=2 ) and cid=1;

///////////////////////////////

select did,count(distinct lid) as count from lecturer group by did;

///////////////////////////////

select s.sid from student s where s.sid not in (select c.sid from cs_relation c where c.cid in  (select l.cid from lc_relation l where l.lid=1));

//////////////////////////////

select avg(score),did from (cs_relation a natural join student r) group by a.cid,r.did;

/////////////////////////////

select max(avg),did from (select avg(score) as avg,cid,did from (cs_relation c join course s using (cid))  group by cid,did) a group by did;
-- OR
(select avg(score) as avg,cid,did from (cs_relation c join course s using (cid))  group by cid,did) as aq natural join (select max(avg) as avg,did from (select avg(score) as avg,cid,did from (cs_relation c join course s using (cid))  group by cid,did) as a group by did) as q;
select * from (select cid,avg(score) as average from cs_relation group by cid) as n natural join
(select did,max(av) as average from (select distinct did,cid,avg(score) as av from course join cs_relation using (cid) group by cid,did) as t group by did) as q;

/////////////////////////////

(select count(*) as count,"F" as grade from cs_relation where score<40 and cid=1) union (select count(*) as count,"B" as grade from cs_relation where score>=40 and cid=1 and score<70) union (select count(*) as count,"A" as grade from cs_relation where score>40 and cid=1);
