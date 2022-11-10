-- phpMyAdmin SQL Dump
-- version 4.4.15.9
-- https://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 31, 2019 at 07:59 PM
-- Server version: 5.6.37
-- PHP Version: 7.1.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `le`
--

-- --------------------------------------------------------

--
-- Table structure for table `cheat_addreses`
--

CREATE TABLE IF NOT EXISTS `cheat_addreses` (
  `title` int(11) NOT NULL,
  `id` int(11) NOT NULL,
  `ind` int(11) NOT NULL,
  `address` varchar(10) NOT NULL,
  `name` varchar(50) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=103 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cheat_addreses`
--

INSERT INTO `cheat_addreses` (`title`, `id`, `ind`, `address`, `name`) VALUES
(1096157387, 1, 0, '0x823091D0', 'Menu_PaintAll'),
(1096157387, 2, 1, '0x8241F100', 'R_AddCmdDrawStretchPic'),
(1096157387, 3, 2, '0x8222E2F8', 'ClientThink_Real'),
(1096157387, 4, 3, '0x822C66A8', 'SV_DropClient'),
(1096157387, 5, 4, '0x822F1588', 'UI_SafeTranslateString'),
(1096157387, 6, 5, '0x8216DAF0', 'CL_WritePacket'),
(1096157387, 7, 6, '0x8232E6D8', 'Dvar_GetUnpackedColorByName'),
(1096157387, 8, 7, '0x82444510', 'R_SetFrameFog'),
(1096157387, 9, 8, '0x82413F48', 'R_RegisterMaterial'),
(1096157387, 10, 9, '0x82407A90', 'R_RegisterFont'),
(1096157387, 11, 10, '0x8241F9E0', 'R_AddCmdDrawText'),
(1096157387, 12, 11, '0x82407B00', 'R_TextWidth'),
(1096157387, 13, 12, '0x8232E2C8', 'Dvar_GetBool'),
(1096157387, 14, 13, '0x8232E488', 'Dvar_GetString'),
(1096157387, 15, 14, '0x82287F68', 'Cbuf_AddText'),
(1096157387, 16, 15, '0x82117668', 'CG_DrawRotatedPicPhysical'),
(1096157387, 17, 16, '0x8211BCB8', 'CG_DObjGetWorldTagPos'),
(1096157387, 18, 17, '0x8216ED28', 'Key_IsDown'),
(1096157387, 19, 18, '0x8215D848', 'CG_CanSeePlayer'),
(1096157387, 20, 19, '0x822A28D0', 'Com_GetClientDObj'),
(1096157387, 21, 20, '0x822B5C98', 'SL_GetString'),
(1096157387, 22, 21, '0x822B5CC8', 'SL_GetString_0'),
(1096157387, 23, 22, '0x822513A0', 'CG_RegisterTag'),
(1096157387, 24, 23, '0x820D3860', 'AimAssist_GetTagPos'),
(1096157387, 25, 24, '0x82109C50', 'CG_WorldPosToCompass'),
(1096157387, 26, 25, '0x82111B88', 'CG_DrawGrenadePointer'),
(1096157387, 27, 26, '0x82111D40', 'CG_DrawGrenadeIcon'),
(1096157387, 28, 27, '0x821631B8', 'CG_Trace'),
(1096157387, 29, 28, '0x82162FA0', 'CG_TraceIgnoreSetup'),
(1096157387, 30, 29, '0x82163278', 'CG_TraceIgnoreSetup2'),
(1096157387, 31, 30, '0x8215D780', 'CG_LocationalTrace'),
(1096157387, 32, 31, '0x82159330', 'BulletTrace'),
(1096157387, 33, 32, '0x8215FB08', 'FireBulletPenetrate'),
(1096157387, 34, 33, '0x82280720', 'Trace_GetEntityHitId'),
(1096157387, 35, 34, '0x82159BE8', 'RandomBulletDir'),
(1096157387, 36, 35, '0x820E9C28', 'BG_SeedRandWithGameTime'),
(1096157387, 37, 36, '0x82101B08', 'BG_GetSpreadForWeapon'),
(1096157387, 38, 37, '0x82159240', 'CG_GetPlayerViewOrigin'),
(1096157387, 39, 38, '0x8216F0E0', 'Key_GetBindingForCmd'),
(1096157387, 40, 39, '0x8216E0B0', 'CL_ExecuteKey'),
(1096157387, 41, 40, '0x8217C4C8', 'CG_GetRankIcon'),
(1096157387, 42, 41, '0x82714228', 'CUSTOM_GetPingData'),
(1096157387, 43, 42, '0x836E5AF8', 'CUSTOM_GetUIContext'),
(1096157387, 44, 43, '0x839691A8', 'CUSTOM_GetXenonUserData'),
(1096157387, 45, 44, '0x82713DC4', 'CUSTOM_GetClientActive'),
(1096157387, 46, 45, '0x8263EE44', 'CUSTOM_GetServerInfo'),
(1096157387, 47, 46, '0x826436B8', 'CUSTOM_GetCEntity'),
(1096157387, 48, 47, '0x8263EE60', 'CUSTOM_GetCG'),
(1096157387, 49, 48, '0x82676C00', 'MISC_CmdMr'),
(1096157387, 50, 49, '0x821614B8', 'PATCH_NoRecoil'),
(1096157387, 51, 50, '0x82124BD0', 'PATCH_Laser'),
(1096157387, 52, 51, '0x8210E58C', 'PATCH_ConstantRadar'),
(1096157387, 53, 52, '0x82144130', 'PATCH_CG_Flashbanged'),
(1096157387, 54, 53, '0x82111688', 'PATCH_CG_DrawFlashDamage'),
(1096157387, 55, 54, '0x82143610', 'PATCH_Flashbang'),
(1096157387, 56, 55, '0x82164EF8', 'PATCH_CL_CapTurnRate'),
(1096157387, 57, 56, '0x82144108', 'PATCH_CG_StartShellShock'),
(1096157387, 58, 57, '0x82143FC8', 'PATCH_CG_UpdateShellShock'),
(1096157387, 59, 58, '0x82103CD8', 'PATCH_NoSway1'),
(1096157387, 60, 59, '0x8210468C', 'PATCH_NoSway2'),
(1096157387, 61, 60, '0x820E5AE8', 'PATCH_NoFlinch'),
(1096157387, 62, 61, '0x82643E60', 'DVAR_HearEveryone1'),
(1096157387, 63, 62, '0x8263EE10', 'DVAR_HearEveryone2'),
(1096157387, 64, 63, '0x8263ED14', 'DVAR_ThirdPerson'),
(1096157387, 65, 64, '0x82643CF0', 'DVAR_ThirdPersonRange'),
(1096157387, 66, 65, '0x82643EC4', 'DVAR_FOV'),
(1096157387, 67, 66, '0x8215F9C0', 'PATCH_AutoWall1'),
(1096157387, 68, 67, '0x8215FDEC', 'PATCH_AutoWall2'),
(1096157387, 69, 68, '0x8215FDF4', 'PATCH_AutoWall3'),
(1096157387, 70, 69, '0x8215FDF8', 'PATCH_AutoWall4'),
(1096157387, 71, 70, '0x82113690', 'PATCH_VisualESPName'),
(1096157387, 72, 71, '0x8263EE14', 'DVAR_cg_hudGrenadeIconOffset'),
(1096157387, 73, 72, '0x8263ED60', 'DVAR_cg_hudGrenadeIconWidth'),
(1096157387, 74, 73, '0x82643DE8', 'DVAR_cg_hudGrenadeIconHeight'),
(1096157387, 75, 74, '0x82643DF0', 'DVAR_cg_hudGrenadePointerWidth'),
(1096157387, 76, 75, '0x82643ED8', 'DVAR_cg_hudGrenadePointerHeight'),
(1096157387, 77, 76, '0x8210AC98', 'CG_CompassDrawPlayerMap'),
(1096157387, 78, 77, '0x8210B540', 'CG_CompassDrawPlayer'),
(1096157387, 79, 78, '0x8210C328', 'CG_CompassDrawCrates'),
(1096157387, 80, 79, '0x8210B818', 'CG_CompassDrawPlanes'),
(1096157387, 81, 80, '0x8210DB68', 'CG_CompassDrawFriendlies'),
(1096157387, 82, 81, '0x8210E3E0', 'CG_CompassDrawEnemies'),
(1096157387, 83, 82, '0x8210E9A8', 'CG_CompassDrawTurrets'),
(1096157387, 84, 83, '0x8210FE10', 'CG_CompassDrawMissiles'),
(1096157387, 85, 84, '0x823AD678', 'CG_CompassDrawHelicopter'),
(1096157387, 86, 85, '0x82109ED4', 'PATCH_CompassOverdrawn'),
(1096157387, 87, 86, '0x822FF05C', 'PATCH_CompassScanLineBorder'),
(1096157387, 88, 87, '0x82132D60', 'PATCH_CG_CompassDrawTickertape1'),
(1096157387, 89, 88, '0x82132D98', 'PATCH_CG_CompassDrawTickertape2'),
(1096157387, 90, 89, '0x82132CF0', 'PATCH_CG_CompassDrawPlayerMap'),
(1096157387, 91, 90, '0x82132DEC', 'PATCH_CG_CompassDrawPlayer'),
(1096157387, 92, 91, '0x82132D28', 'PATCH_CG_CompassDrawCrates'),
(1096157387, 93, 92, '0x82132DB4', 'PATCH_CG_CompassDrawFriendlies'),
(1096157387, 94, 93, '0x82132DD0', 'PATCH_CG_CompassDrawEnemies'),
(1096157387, 95, 94, '0x82132E2C', 'PATCH_CG_CompassDrawTurrets'),
(1096157387, 96, 95, '0x82132E10', 'PATCH_CG_CompassDrawMissiles'),
(1096157387, 97, 96, '0x82133340', 'PATCH_CG_CompassDrawHelicopter'),
(1096157387, 98, 97, '0x8261E0A4', 'DVAR_CompassMaxRange'),
(1096157387, 99, 98, '0x8257FF4C', 'XamInputGetState'),
(1096157387, 100, 99, '0x820F7818', 'BG_GetWeaponName'),
(1096157387, 101, 100, '0x82159C70', 'CG_BulletEndPos'),
(1096157387, 102, 101, '0x82135BC0', 'CG_Respawn');

-- --------------------------------------------------------

--
-- Table structure for table `cheat_data`
--

CREATE TABLE IF NOT EXISTS `cheat_data` (
  `id` int(11) NOT NULL,
  `title` int(11) NOT NULL DEFAULT '0',
  `latest_version` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cheat_data`
--

INSERT INTO `cheat_data` (`id`, `title`, `latest_version`) VALUES
(1, 1096157387, 1);

-- --------------------------------------------------------

--
-- Table structure for table `cheat_endpoints`
--

CREATE TABLE IF NOT EXISTS `cheat_endpoints` (
  `id` int(11) NOT NULL,
  `token` varchar(64) NOT NULL DEFAULT '',
  `cpu` varchar(32) NOT NULL,
  `last_heartbeat` int(11) NOT NULL DEFAULT '0',
  `welcome_time` int(11) NOT NULL DEFAULT '0',
  `main_connection_index` int(11) NOT NULL DEFAULT '0',
  `has_received_presence` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `cheat_functions`
--

CREATE TABLE IF NOT EXISTS `cheat_functions` (
  `id` int(11) NOT NULL,
  `title` int(11) NOT NULL,
  `ind` int(11) NOT NULL,
  `bytes` varchar(1000) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cheat_functions`
--

INSERT INTO `cheat_functions` (`id`, `title`, `ind`, `bytes`) VALUES
(1, 1096157387, 0, '2B0300004D9A00202B0400004D9A0020548B843E548A04213D6B3D6041820008396B000191630000548A043E3D607D693D204E80616B03A63D4A396B6129042091630008914300049123000C4E800020');

-- --------------------------------------------------------

--
-- Table structure for table `cheat_natives`
--

CREATE TABLE IF NOT EXISTS `cheat_natives` (
  `id` int(11) NOT NULL,
  `title` int(11) NOT NULL,
  `ind` int(11) NOT NULL,
  `address` varchar(11) NOT NULL,
  `name` varchar(50) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cheat_natives`
--

INSERT INTO `cheat_natives` (`id`, `title`, `ind`, `address`, `name`) VALUES
(1, 1096157387, 0, '0x956386aa', 'NATIVE_GetCG'),
(2, 1096157387, 1, '0xf342a904', 'NATIVE_GetCEntity'),
(3, 1096157387, 2, '0x83e2154f', 'NATIVE_GetClientInfo'),
(4, 1096157387, 3, '0xce5074be', 'NATIVE_GetServerInfo'),
(5, 1096157387, 4, '0xef1ecfc0', 'NATIVE_GetClientActive'),
(6, 1096157387, 5, '0x9bc389d1', 'NATIVE_GetXenonUserData'),
(7, 1096157387, 6, '0x2fe535c0', 'NATIVE_GetUIContext'),
(8, 1096157387, 7, '0x3fc9862e', 'NATIVE_GetPingData'),
(9, 1096157387, 8, '0xd1492361', 'NATIVE_AimAssist_GetTagPos'),
(10, 1096157387, 9, '0xc9d1c6c7', 'NATIVE_CG_RegisterTag'),
(11, 1096157387, 10, '0x2da97d1a', 'NATIVE_R_RegisterMaterial'),
(12, 1096157387, 11, '0xb5675197', 'NATIVE_R_RegisterFont'),
(13, 1096157387, 12, '0xfd8bec3', 'NATIVE_R_AddCmdDrawStretchPic'),
(14, 1096157387, 13, '0xd14e6bec', 'NATIVE_R_AddCmdDrawText'),
(15, 1096157387, 14, '0xeb99411d', 'NATIVE_R_TextWidth'),
(16, 1096157387, 15, '0x2d8b17ed', 'NATIVE_Dvar_GetBool'),
(17, 1096157387, 16, '0xcbebc73f', 'NATIVE_Dvar_GetString'),
(18, 1096157387, 17, '0xc7eb0856', 'NATIVE_Cbuf_AddText'),
(19, 1096157387, 18, '0x31a7472', 'NATIVE_CG_DrawRotatedPicPhysical'),
(20, 1096157387, 19, '0x3ca659c9', 'NATIVE_Com_GetClientDObj'),
(21, 1096157387, 20, '0xb3d7f226', 'NATIVE_CG_DObjGetWorldTagPos'),
(22, 1096157387, 21, '0x672f07a7', 'NATIVE_Key_IsDown'),
(23, 1096157387, 22, '0x81a87e79', 'NATIVE_CG_CanSeePlayer'),
(24, 1096157387, 23, '0x38319d76', 'NATIVE_GetTagPosition'),
(25, 1096157387, 24, '0x3cd1fcc7', 'NATIVE_CG_WorldPosToCompass'),
(26, 1096157387, 25, '0xc4a91723', 'NATIVE_CG_DrawGrenadePointer'),
(27, 1096157387, 26, '0x54a3225a', 'NATIVE_CG_DrawGrenadeIcon'),
(28, 1096157387, 27, '0x9ccb8548', 'NATIVE_CG_TraceIgnoreSetup'),
(29, 1096157387, 28, '0x50ad5829', 'NATIVE_CG_TraceIgnoreSetup2'),
(30, 1096157387, 29, '0x72560175', 'NATIVE_CG_Trace'),
(31, 1096157387, 30, '0x63046d4d', 'NATIVE_CG_LocationalTrace'),
(32, 1096157387, 31, '0x49a5b981', 'NATIVE_BulletTrace'),
(33, 1096157387, 32, '0x9d9e6cca', 'NATIVE_FireBulletPenetrate'),
(34, 1096157387, 33, '0x29a858ed', 'NATIVE_Trace_GetEntityHitId'),
(35, 1096157387, 34, '0x81d948fb', 'NATIVE_RandomBulletDir'),
(36, 1096157387, 35, '0x483c2d4d', 'NATIVE_BG_SeedRandWithGameTime'),
(37, 1096157387, 36, '0x9c27d046', 'NATIVE_BG_GetSpreadForWeapon'),
(38, 1096157387, 37, '0xd0d6718d', 'NATIVE_SL_GetString'),
(39, 1096157387, 38, '0x7583c6f4', 'NATIVE_SL_GetString_0'),
(40, 1096157387, 39, '0x30f2cc87', 'NATIVE_CG_GetPlayerViewOrigin'),
(41, 1096157387, 40, '0x64c04760', 'NATIVE_R_TextHeight'),
(42, 1096157387, 41, '0x98e7ea67', 'NATIVE_Key_GetBindingForCmd'),
(43, 1096157387, 42, '0x9f59c64d', 'NATIVE_CL_ExecuteKey'),
(44, 1096157387, 43, '0xf3ace6b7', 'NATIVE_CG_GetRankIcon'),
(45, 1096157387, 44, '0x95fb7cde', 'NATIVE_CG_CompassDrawPlayerMap'),
(46, 1096157387, 45, '0xb7bc8f04', 'NATIVE_CG_CompassDrawPlayer'),
(47, 1096157387, 46, '0x60fd192e', 'NATIVE_CG_CompassDrawCrates'),
(48, 1096157387, 47, '0xb5470ce8', 'NATIVE_CG_CompassDrawPlanes'),
(49, 1096157387, 48, '0xb5e00894', 'NATIVE_CG_CompassDrawFriendlies'),
(50, 1096157387, 49, '0xfaeca98', 'NATIVE_CG_CompassDrawEnemies'),
(51, 1096157387, 50, '0x6efea031', 'NATIVE_CG_CompassDrawMissiles'),
(52, 1096157387, 51, '0x22b97388', 'NATIVE_CG_CompassDrawHelicopter'),
(53, 1096157387, 52, '0x201f8120', 'NATIVE_CG_CompassDrawTurrets'),
(54, 1096157387, 53, '0x4934717a', 'NATIVE_BG_GetWeaponName'),
(55, 1096157387, 54, '0xe913ed2', 'NATIVE_CG_BulletEndPos');

-- --------------------------------------------------------

--
-- Table structure for table `clients`
--

CREATE TABLE IF NOT EXISTS `clients` (
  `id` int(11) NOT NULL,
  `cpu` varchar(32) DEFAULT NULL,
  `has_used_before` tinyint(1) NOT NULL DEFAULT '0',
  `authstatus` int(11) NOT NULL,
  `bannedreason` varchar(255) DEFAULT 'Not Set'
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `clients`
--

INSERT INTO `clients` (`id`, `cpu`, `has_used_before`, `authstatus`, `bannedreason`) VALUES
(2, 'FB82515817F4B81F0D8CF0F59CB4F8A5', 1, 3, 'Not Set'),
(3, '01010101010101010101010101010101', 0, 0, 'Not Set');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `cheat_addreses`
--
ALTER TABLE `cheat_addreses`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cheat_data`
--
ALTER TABLE `cheat_data`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cheat_endpoints`
--
ALTER TABLE `cheat_endpoints`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cheat_functions`
--
ALTER TABLE `cheat_functions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cheat_natives`
--
ALTER TABLE `cheat_natives`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `clients`
--
ALTER TABLE `clients`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `cheat_addreses`
--
ALTER TABLE `cheat_addreses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=103;
--
-- AUTO_INCREMENT for table `cheat_data`
--
ALTER TABLE `cheat_data`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `cheat_endpoints`
--
ALTER TABLE `cheat_endpoints`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `cheat_functions`
--
ALTER TABLE `cheat_functions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `cheat_natives`
--
ALTER TABLE `cheat_natives`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=56;
--
-- AUTO_INCREMENT for table `clients`
--
ALTER TABLE `clients`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=4;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
