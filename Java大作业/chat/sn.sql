
CREATE DATABASE wareHouseManagerSystem DEFAULT CHARACTER SET utf8;

CREATE TABLE `goods` (
  `id` bigint(20) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `num` varchar(255) NOT NULL DEFAULT '',
  `branch` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `purchase` (
  `id` bigint(20) DEFAULT NULL,
  `purchaseNum` varchar(255) NOT NULL DEFAULT '',
  `purchasePersonName` varchar(255) DEFAULT NULL,
  `linkPersonName` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`purchaseNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `purchaseitem` (
  `id` bigint(20) DEFAULT NULL,
  `purchaseNum` varchar(255) DEFAULT NULL,
  `itemNum` varchar(255) NOT NULL DEFAULT '',
  `goodsNum` varchar(255) DEFAULT NULL,
  `size` int(11) DEFAULT NULL,
  `price` int(11) DEFAULT NULL,
  `purchaseDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`itemNum`),
  KEY `purchaseNum` (`purchaseNum`),
  KEY `goodsNum` (`goodsNum`),
  CONSTRAINT `purchaseitem_ibfk_1` FOREIGN KEY (`purchaseNum`) REFERENCES `purchase` (`purchaseNum`),
  CONSTRAINT `purchaseitem_ibfk_2` FOREIGN KEY (`goodsNum`) REFERENCES `goods` (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `repository` (
  `id` varchar(200) DEFAULT NULL,
  `goodsNum` varchar(255) NOT NULL DEFAULT '',
  `size` int(11) DEFAULT NULL,
  PRIMARY KEY (`goodsNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `sell` (
  `id` bigint(20) DEFAULT NULL,
  `sellNum` varchar(255) NOT NULL DEFAULT '',
  `buyPersonName` varchar(255) DEFAULT NULL,
  `note` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`sellNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `sellitem` (
  `id` bigint(20) DEFAULT NULL,
  `sellNum` varchar(255) DEFAULT NULL,
  `sellItemNum` varchar(255) NOT NULL DEFAULT '',
  `goodsNum` varchar(255) DEFAULT NULL,
  `size` int(11) DEFAULT NULL,
  `sellPrice` int(11) DEFAULT NULL,
  `sellDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`sellItemNum`),
  KEY `sellNum` (`sellNum`),
  KEY `bookNum` (`goodsNum`),
  CONSTRAINT `sellitem_ibfk_1` FOREIGN KEY (`sellNum`) REFERENCES `sell` (`sellNum`),
  CONSTRAINT `sellitem_ibfk_2` FOREIGN KEY (`goodsNum`) REFERENCES `goods` (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `user` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `userName` varchar(255) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;


insert into user value(1, 11, 11);