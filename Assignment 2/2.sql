CREATE DATABASE Ass2_4;
USE Ass2_4;

CREATE TABLE department(did int NOT NULL UNIQUE,name varchar(50),PRIMARY KEY (did));

CREATE TABLE lecturer(lid int NOT NULL UNIQUE,name varchar(50) NOT NULL,age int,did int, PRIMARY KEY (lid,did),FOREIGN KEY (did) REFERENCES department (did) );


CREATE TABLE student(sid int NOT NULL UNIQUE,name varchar(50) NOT NULL,age int,did int,CONSTRAINT PRIMARY KEY	(sid),FOREIGN KEY (did) REFERENCES department (did)  );


CREATE TABLE course(cid int NOT NULL UNIQUE,name varchar(50) NOT NULL,did int,CONSTRAINT PRIMARY KEY (cid,did),FOREIGN KEY (did) REFERENCES department (did) );



CREATE TABLE lc_relation(lid int NOT NULL,cid int NOT NULL,did int NOT NULL, FOREIGN KEY (lid, did) REFERENCES lecturer (lid, did), FOREIGN KEY (cid, did) REFERENCES course (cid, did));


-- CREATE TABLE cs_relation(sid int NOT NULL,cid int NOT NULL,FOREIGN KEY (sid) REFERENCES student (sid) ,FOREIGN KEY (cid) REFERENCES course (cid) );

CREATE TABLE cs_relation ( sid int NOT NULL, cid int NOT NULL, PRIMARY KEY(sid,cid) ,FOREIGN KEY (sid) REFERENCES student (sid), FOREIGN KEY (cid) REFERENCES course (cid));



INSERT INTO department (did,name) VALUES (1,'CSE');
INSERT INTO department (did,name) VALUES (2,'MNC');


INSERT INTO lecturer (lid,name,age,did) VALUES (1,'Inkulu',45,1);

INSERT INTO lecturer (lid,name,age,did) VALUES (2,'ABC',40,2);

INSERT INTO course (cid,name,did) VALUES (1,'CS101',1);
INSERT INTO course (cid,name,did) VALUES (2,'MA101',2);

INSERT INTO student (sid,name,age,did) VALUES (1,'Mukul',20,1);
INSERT INTO student (sid,name,age,did) VALUES (2,'Paranjay',21,2);




delimiter #
mysql> create trigger report_error before insert on lc_relation
    -> for each row
    -> begin
    -> if (select did from lecturer where lid=new.lid)!=(select did from course where cid=new.cid)
    -> then
    ->  SIGNAL SQLSTATE '45000'
    ->                     SET MESSAGE_TEXT = 'Cannot add or update row: only one active row allowed per type';
    ->           END IF;
    ->      END;
    -> #
Query OK, 0 rows affected (0.00 sec)

mysql> delimiter ;





delimiter #
mysql> create trigger report_error1 before update on lc_relation
    -> for each row
    -> begin
    -> if (select did from lecturer where lid=new.lid)!=(select did from course where cid=new.cid)
    -> then
    ->  SIGNAL SQLSTATE '45000'
    ->                     SET MESSAGE_TEXT = 'department of lecturer and Course do not match';
    ->           END IF;
    ->      END;
    -> #
Query OK, 0 rows affected (0.00 sec)

mysql> delimiter ;




Q1: select name from student where sid in ( select sid from cs_relation where cid=2);
   -- or
select distinct s.name from student s,cs_relation sc where s.sid = sc.sid && sc.cid=2;

Q2: select name from student where sid in ( select sid from cs_relation where cid in (select cid from lc_relation where lid =2));
-- or
select distinct s.name from student s,cs_relation sc,lc_relation lc where s.sid=sc.sid && sc.cid=lc.cid && lc.lid=2;


Q3; select distinct c.name from course c,student s, cs_relation sc where c.did != s.did && c.cid = sc.cid && sc.sid= s.sid && s.sid = 1;


insert into cs_relation (sid,cid) values (1.23);

delete from course where cid =1;




---------------------------------------------------


select distinct c.cid,c.name from course c,cs_relation sc where c.cid = sc.cid && sc.sid = 1;


select name from student where sid in (select sid from cs_relation where cid in (select cid from cs_relation where sid=3));


SELECT * FROM course WHERE did=2 AND cid IN (SELECT cid FROM cs_relation WHERE sid IN (SELECT sid FROM student WHERE did!=2));


select distinct s.name from student s,cs_relation sc where sc.marks < 20  && sc.marks > 5 && s.sid=sc.sid && sc.cid = 1;


update lc_relation set cid=2 where lid =1;



//////////////////////////////////////////////////////////////////////////////////
