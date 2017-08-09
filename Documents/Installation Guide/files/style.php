/*  phpBB3 Style Sheet
    --------------------------------------------------------------
	Style name:			prosilver (the default phpBB 3.0.x style)
	Based on style:		
	Original author:	Tom Beddard ( http://www.subblue.com/ )
	Modified by:		phpBB Group ( http://www.phpbb.com/ )
    --------------------------------------------------------------
*/

/* General Markup Styles
---------------------------------------- */

* {
	/* Reset browsers default margin, padding and font sizes */
	margin: 0;
	padding: 0;
}

html {
	font-size: 100%;
	/* Always show a scrollbar for short pages - stops the jump when the scrollbar appears. non-IE browsers */
	height: 101%;
}

body {
	/* Text-Sizing with ems: http://www.clagnut.com/blog/348/ */
	font-family: Verdana, Helvetica, Arial, sans-serif;
	color: #828282;
	background-color: #FFFFFF;
	/*font-size: 62.5%;			 This sets the default font size to be equivalent to 10px */
	font-size: 10px;
	margin: 0;
	padding: 0;
}

h1 {
	/* Forum name */
	font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
	margin-right: 200px;
	color: #FFFFFF;
	margin-top: 15px;
	font-weight: bold;
	font-size: 2em;
}

h2 {
	/* Forum header titles */
	font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
	font-weight: normal;
	color: #3f3f3f;
	font-size: 2em;
	margin: 0.8em 0 0.2em 0;
}

h2.solo {
	margin-bottom: 1em;
}

h3 {
	/* Sub-headers (also used as post headers, but defined later) */
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	text-transform: uppercase;
	border-bottom: 1px solid #CCCCCC;
	margin-bottom: 3px;
	padding-bottom: 2px;
	font-size: 1.05em;
	color: #989898;
	margin-top: 20px;
}

h4 {
	/* Forum and topic list titles */
	font-family: "Trebuchet MS", Verdana, Helvetica, Arial, Sans-serif;
	font-size: 1.3em;
}

p {
	line-height: 1.3em;
	font-size: 1.1em;
	margin-bottom: 1.5em;
}

img {
	border-width: 0;
}

hr {
	/* Also see tweaks.css */
	border: 0 none #FFFFFF;
	border-top: 1px solid #CCCCCC;
	height: 1px;
	margin: 5px 0;
	display: block;
	clear: both;
}

hr.dashed {
	border-top: 1px dashed #CCCCCC;
	margin: 10px 0;
}

hr.divider {
	display: none;
}

p.right {
	text-align: right;
}

/* Main blocks
---------------------------------------- */
#wrap {
	padding: 0px;
	width:753px;
	min-width: 600px;
}

#simple-wrap {
	padding: 6px 10px;
}

#page-body {
	margin: 4px 0;
	clear: both;
}

#page-footer {
	clear: both;
}

#page-footer h3 {
	margin-top: 20px;
}

#logo {
	float: left;
	width: auto;
	padding: 10px 13px 0 10px;
}

a#logo:hover {
	text-decoration: none;
}

/* Search box
--------------------------------------------- */
#search-box {
	color: #FFFFFF;
	position: relative;
	margin-top: 30px;
	margin-right: 5px;
	display: block;
	float: right;
	text-align: right;
	white-space: nowrap; /* For Opera */
}

#search-box #keywords {
	width: 95px;
	background-color: #FFF;
}

#search-box input {
	border: 1px solid #b0b0b0;
}

/* .button1 style defined later, just a few tweaks for the search button version */
#search-box input.button1 {
	padding: 1px 5px;
}

#search-box li {
	text-align: right;
	margin-top: 4px;
}

#search-box img {
	vertical-align: middle;
	margin-right: 3px;
}

/* Site description and logo */
#site-description {
	float: left;
	width: 70%;
}

#site-description h1 {
	margin-right: 0;
}

/* Round cornered boxes and backgrounds
---------------------------------------- */
.headerbar {
	background: #ebebeb none repeat-x 0 0;
	color: #FFFFFF;
	margin-bottom: 4px;
	padding: 0 5px;
}

.navbar {
	background-color: #ebebeb;
	padding: 0 10px;
}

.forabg {
	background: #b1b1b1 none repeat-x 0 0;
	margin-bottom: 4px;
	padding: 0 5px;
	clear: both;
}

.forumbg {
	background: #ebebeb none repeat-x 0 0;
	margin-bottom: 4px;
	padding: 0 5px;
	clear: both;
}

.panel {
	margin-bottom: 4px;
	padding: 0 10px;
	background-color: #f3f3f3;
	color: #3f3f3f;
}

.post {
	padding: 0 10px;
	margin-bottom: 4px;
	background-repeat: no-repeat;
	background-position: 100% 0;
}

.post:target .content {
	color: #000000;
}

.post:target h3 a {
	color: #000000;
}

.bg1	{ background-color: #f7f7f7;}
.bg2	{ background-color: #f2f2f2; }
.bg3	{ background-color: #ebebeb; }

.rowbg {
	margin: 5px 5px 2px 5px;
}

.ucprowbg {
	background-color: #e2e2e2;
}

.fieldsbg {
	/*border: 1px #DBDEE2 solid;*/
	background-color: #eaeaea;
}

span.corners-top, span.corners-bottom, span.corners-top span, span.corners-bottom span {
	font-size: 1px;
	line-height: 1px;
	display: block;
	height: 5px;
	background-repeat: no-repeat;
}

span.corners-top {
	background-image: none;
	background-position: 0 0;
	margin: 0 -5px;
}

span.corners-top span {
	background-image: none;
	background-position: 100% 0;
}

span.corners-bottom {
	background-image: none;
	background-position: 0 100%;
	margin: 0 -5px;
	clear: both;
}

span.corners-bottom span {
	background-image: none;
	background-position: 100% 100%;
}

.headbg span.corners-bottom {
	margin-bottom: -1px;
}

.post span.corners-top, .post span.corners-bottom, .panel span.corners-top, .panel span.corners-bottom, .navbar span.corners-top, .navbar span.corners-bottom {
	margin: 0 -10px;
}

.rules span.corners-top {
	margin: 0 -10px 5px -10px;
}

.rules span.corners-bottom {
	margin: 5px -10px 0 -10px;
}

/* Horizontal lists
----------------------------------------*/
ul.linklist {
	display: block;
	margin: 0;
}

ul.linklist li {
	display: block;
	list-style-type: none;
	float: left;
	width: auto;
	margin-right: 5px;
	font-size: 1.1em;
	line-height: 2.2em;
}

ul.linklist li.rightside, p.rightside {
	float: right;
	margin-right: 0;
	margin-left: 5px;
	text-align: right;
}

ul.navlinks {
	padding-bottom: 1px;
	margin-bottom: 1px;
	border-bottom: 1px solid #FFFFFF;
	font-weight: bold;
}

ul.leftside {
	float: left;
	margin-left: 0;
	margin-right: 5px;
	text-align: left;
}

ul.rightside {
	float: right;
	margin-left: 5px;
	margin-right: -5px;
	text-align: right;
}

/* Table styles
----------------------------------------*/
table.table1 {
	/* See tweaks.css */
}

#ucp-main table.table1 {
	padding: 2px;
}

table.table1 thead th {
	font-weight: normal;
	text-transform: uppercase;
	color: #FFFFFF;
	line-height: 1.3em;
	font-size: 1em;
	padding: 0 0 4px 3px;
}

table.table1 thead th span {
	padding-left: 7px;
}

table.table1 tbody tr {
	border: 1px solid #cfcfcf;
}

table.table1 tbody tr:hover, table.table1 tbody tr.hover {
	background-color: #f6f6f6;
	color: #000;
}

table.table1 td {
	color: #6a6a6a;
	font-size: 1.1em;
}

table.table1 tbody td {
	padding: 5px;
	border-top: 1px solid #FAFAFA;
}

table.table1 tbody th {
	padding: 5px;
	border-bottom: 1px solid #000000;
	text-align: left;
	color: #333333;
	background-color: #FFFFFF;
}

/* Specific column styles */
table.table1 .name		{ text-align: left; }
table.table1 .posts		{ text-align: center !important; width: 7%; }
table.table1 .joined	{ text-align: left; width: 15%; }
table.table1 .active	{ text-align: left; width: 15%; }
table.table1 .mark		{ text-align: center; width: 7%; }
table.table1 .info		{ text-align: left; width: 30%; }
table.table1 .info div	{ width: 100%; white-space: normal; overflow: hidden; }
table.table1 .autocol	{ line-height: 2em; white-space: nowrap; }
table.table1 thead .autocol { padding-left: 1em; }

table.table1 span.rank-img {
	float: right;
	width: auto;
}

table.info td {
	padding: 3px;
}

table.info tbody th {
	padding: 3px;
	text-align: right;
	vertical-align: top;
	color: #000000;
	font-weight: normal;
}

.forumbg table.table1 {
	margin: 0 -2px -1px -1px;
}

/* Misc layout styles
---------------------------------------- */
/* column[1-2] styles are containers for two column layouts 
   Also see tweaks.css */
.column1 {
	float: left;
	clear: left;
	width: 49%;
}

.column2 {
	float: right;
	clear: right;
	width: 49%;
}

/* General classes for placing floating blocks */
.left-box {
	float: left;
	width: auto;
	text-align: left;
}

.right-box {
	float: right;
	width: auto;
	text-align: right;
}

dl.details {
	/*font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;*/
	font-size: 1.1em;
}

dl.details dt {
	float: left;
	clear: left;
	width: 30%;
	text-align: right;
	color: #000000;
	display: block;
}

dl.details dd {
	margin-left: 0;
	padding-left: 5px;
	margin-bottom: 5px;
	color: #828282;
	float: left;
	width: 65%;
}

/* Pagination
---------------------------------------- */
.pagination {
	height: 1%; /* IE tweak (holly hack) */
	width: auto;
	text-align: right;
	margin-top: 5px;
	float: right;
}

.pagination span.page-sep {
	display: none;
}

li.pagination {
	margin-top: 0;
}

.pagination strong, .pagination b {
	font-weight: normal;
}

.pagination span strong {
	padding: 0 2px;
	margin: 0 2px;
	font-weight: normal;
	color: #FFFFFF;
	background-color: #bfbfbf;
	border: 1px solid #bfbfbf;
	font-size: 0.9em;
}

.pagination span a, .pagination span a:link, .pagination span a:visited, .pagination span a:active {
	font-weight: normal;
	text-decoration: none;
	color: #747474;
	margin: 0 2px;
	padding: 0 2px;
	background-color: #eeeeee;
	border: 1px solid #bababa;
	font-size: 0.9em;
	line-height: 1.5em;
}

.pagination span a:hover {
	border-color: #d2d2d2;
	background-color: #d2d2d2;
	color: #FFF;
	text-decoration: none;
}

.pagination img {
	vertical-align: middle;
}

/* Pagination in viewforum for multipage topics */
.row .pagination {
	display: block;
	float: right;
	width: auto;
	margin-top: 0;
	padding: 1px 0 1px 15px;
	font-size: 0.9em;
	background: none 0 50% no-repeat;
}

.row .pagination span a, li.pagination span a {
	background-color: #FFFFFF;
}

.row .pagination span a:hover, li.pagination span a:hover {
	background-color: #d2d2d2;
}

/* Miscellaneous styles
---------------------------------------- */
#forum-permissions {
	float: right;
	width: auto;
	padding-left: 5px;
	margin-left: 5px;
	margin-top: 10px;
	text-align: right;
}

.copyright {
	padding: 5px;
	text-align: center;
	color: #555555;
}

.small {
	font-size: 0.9em !important;
}

.titlespace {
	margin-bottom: 15px;
}

.headerspace {
	margin-top: 20px;
}

.error {
	color: #bcbcbc;
	font-weight: bold;
	font-size: 1em;
}

.reported {
	background-color: #f7f7f7;
}

li.reported:hover {
	background-color: #ececec;
}

div.rules {
	background-color: #ececec;
	color: #bcbcbc;
	padding: 0 10px;
	margin: 10px 0;
	font-size: 1.1em;
}

div.rules ul, div.rules ol {
	margin-left: 20px;
}

p.rules {
	background-color: #ececec;
	background-image: none;
	padding: 5px;
}

p.rules img {
	vertical-align: middle;
	padding-top: 5px;
}

p.rules a {
	vertical-align: middle;
	clear: both;
}

#top {
	position: absolute;
	top: -20px;
}

.clear {
	display: block;
	clear: both;
	font-size: 1px;
	line-height: 1px;
	background: transparent;
}
/* Link Styles
---------------------------------------- */

/* Links adjustment to correctly display an order of rtl/ltr mixed content */
a {
	direction: ltr;
	unicode-bidi: embed;
}

a:link	{ color: #898989; text-decoration: none; }
a:visited	{ color: #898989; text-decoration: none; }
a:hover	{ color: #d3d3d3; text-decoration: underline; }
a:active	{ color: #d2d2d2; text-decoration: none; }

/* Coloured usernames */
.username-coloured {
	font-weight: bold;
	display: inline !important;
	padding: 0 !important;
}

/* Links on gradient backgrounds */
#search-box a:link, .navbg a:link, .forumbg .header a:link, .forabg .header a:link, th a:link {
	color: #FFFFFF;
	text-decoration: none;
}

#search-box a:visited, .navbg a:visited, .forumbg .header a:visited, .forabg .header a:visited, th a:visited {
	color: #FFFFFF;
	text-decoration: none;
}

#search-box a:hover, .navbg a:hover, .forumbg .header a:hover, .forabg .header a:hover, th a:hover {
	color: #ffffff;
	text-decoration: underline;
}

#search-box a:active, .navbg a:active, .forumbg .header a:active, .forabg .header a:active, th a:active {
	color: #ffffff;
	text-decoration: none;
}

/* Links for forum/topic lists */
a.forumtitle {
	font-family: "Trebuchet MS", Helvetica, Arial, Sans-serif;
	font-size: 1.2em;
	font-weight: bold;
	color: #898989;
	text-decoration: none;
}

/* a.forumtitle:visited { color: #898989; } */

a.forumtitle:hover {
	color: #bcbcbc;
	text-decoration: underline;
}

a.forumtitle:active {
	color: #898989;
}

a.topictitle {
	font-family: "Trebuchet MS", Helvetica, Arial, Sans-serif;
	font-size: 1.2em;
	font-weight: bold;
	color: #898989;
	text-decoration: none;
}

/* a.topictitle:visited { color: #d2d2d2; } */

a.topictitle:hover {
	color: #bcbcbc;
	text-decoration: underline;
}

a.topictitle:active {
	color: #898989;
}

/* Post body links */
.postlink {
	text-decoration: none;
	color: #d2d2d2;
	border-bottom: 1px solid #d2d2d2;
	padding-bottom: 0;
}

/* .postlink:visited { color: #bdbdbd; } */

.postlink:active {
	color: #d2d2d2;
}

.postlink:hover {
	background-color: #f6f6f6;
	text-decoration: none;
	color: #404040;
}

.signature a, .signature a:visited, .signature a:hover, .signature a:active {
	border: none;
	text-decoration: underline;
	background-color: transparent;
}

/* Profile links */
.postprofile a:link, .postprofile a:visited, .postprofile dt.author a {
	font-weight: bold;
	color: #898989;
	text-decoration: none;
}

.postprofile a:hover, .postprofile dt.author a:hover {
	text-decoration: underline;
	color: #d3d3d3;
}

/* CSS spec requires a:link, a:visited, a:hover and a:active rules to be specified in this order. */
/* See http://www.phpbb.com/bugs/phpbb3/59685 */
.postprofile a:active {
	font-weight: bold;
	color: #898989;
	text-decoration: none;
}


/* Profile searchresults */	
.search .postprofile a {
	color: #898989;
	text-decoration: none; 
	font-weight: normal;
}

.search .postprofile a:hover {
	color: #d3d3d3;
	text-decoration: underline; 
}

/* Back to top of page */
.back2top {
	clear: both;
	height: 11px;
	text-align: right;
}

a.top {
	background: none no-repeat top left;
	text-decoration: none;
	width: 11px;
	height: 11px;
	display: block;
	float: right;
	overflow: hidden;
	letter-spacing: 1000px;
	text-indent: 11px;
}

a.top2 {
	background: none no-repeat 0 50%;
	text-decoration: none;
	padding-left: 15px;
}

/* Arrow links  */
a.up		{ background: none no-repeat left center; }
a.down		{ background: none no-repeat right center; }
a.left		{ background: none no-repeat 3px 60%; }
a.right		{ background: none no-repeat 95% 60%; }

a.up, a.up:link, a.up:active, a.up:visited {
	padding-left: 10px;
	text-decoration: none;
	border-bottom-width: 0;
}

a.up:hover {
	background-position: left top;
	background-color: transparent;
}

a.down, a.down:link, a.down:active, a.down:visited {
	padding-right: 10px;
}

a.down:hover {
	background-position: right bottom;
	text-decoration: none;
}

a.left, a.left:active, a.left:visited {
	padding-left: 12px;
}

a.left:hover {
	color: #d2d2d2;
	text-decoration: none;
	background-position: 0 60%;
}

a.right, a.right:active, a.right:visited {
	padding-right: 12px;
}

a.right:hover {
	color: #d2d2d2;
	text-decoration: none;
	background-position: 100% 60%;
}

/* invisible skip link, used for accessibility  */
.skiplink {
	position: absolute;
	left: -999px;
	width: 990px;
}

/* Feed icon in forumlist_body.html */
a.feed-icon-forum {
	float: right;
	margin: 3px;
}
/* Content Styles
---------------------------------------- */

ul.topiclist {
	display: block;
	list-style-type: none;
	margin: 0;
}

ul.forums {
	background: #f9f9f9 none repeat-x 0 0;
}

ul.topiclist li {
	display: block;
	list-style-type: none;
	color: #777777;
	margin: 0;
}

ul.topiclist dl {
	position: relative;
}

ul.topiclist li.row dl {
	padding: 2px 0;
}

ul.topiclist dt {
	display: block;
	float: left;
	width: 50%;
	font-size: 1.1em;
	padding-left: 5px;
	padding-right: 5px;
}

ul.topiclist dd {
	display: block;
	float: left;
	border-left: 1px solid #FFFFFF;
	padding: 4px 0;
}

ul.topiclist dfn {
	/* Labels for post/view counts */
	position: absolute;
	left: -999px;
	width: 990px;
}

ul.topiclist li.row dt a.subforum {
	background-image: none;
	background-position: 0 50%;
	background-repeat: no-repeat;
	position: relative;
	white-space: nowrap;
	padding: 0 0 0 12px;
}

.forum-image {
	float: left;
	padding-top: 5px;
	margin-right: 5px;
}

li.row {
	border-top: 1px solid #FFFFFF;
	border-bottom: 1px solid #8f8f8f;
}

li.row strong {
	font-weight: normal;
	color: #000000;
}

li.row:hover {
	background-color: #f6f6f6;
}

li.row:hover dd {
	border-left-color: #CCCCCC;
}

li.header dt, li.header dd {
	line-height: 1em;
	border-left-width: 0;
	margin: 2px 0 4px 0;
	color: #FFFFFF;
	padding-top: 2px;
	padding-bottom: 2px;
	font-size: 1em;
	font-family: Arial, Helvetica, sans-serif;
	text-transform: uppercase;
}

li.header dt {
	font-weight: bold;
}

li.header dd {
	margin-left: 1px;
}

li.header dl.icon {
	min-height: 0;
}

li.header dl.icon dt {
	/* Tweak for headers alignment when folder icon used */
	padding-left: 0;
	padding-right: 50px;
}

/* Forum list column styles */
dl.icon {
	min-height: 35px;
	background-position: 10px 50%;		/* Position of folder icon */
	background-repeat: no-repeat;
}

dl.icon dt {
	padding-left: 45px;					/* Space for folder icon */
	background-repeat: no-repeat;
	background-position: 5px 95%;		/* Position of topic icon */
}

dd.posts, dd.topics, dd.views {
	width: 8%;
	text-align: center;
	line-height: 2.2em;
	font-size: 1.2em;
}

/* List in forum description */
dl.icon dt ol,
dl.icon dt ul {
	list-style-position: inside;
	margin-left: 1em;
}

dl.icon dt li {
	display: list-item;
	list-style-type: inherit;
}

dd.lastpost {
	width: 25%;
	font-size: 1.1em;
}

dd.redirect {
	font-size: 1.1em;
	line-height: 2.5em;
}

dd.moderation {
	font-size: 1.1em;
}

dd.lastpost span, ul.topiclist dd.searchby span, ul.topiclist dd.info span, ul.topiclist dd.time span, dd.redirect span, dd.moderation span {
	display: block;
	padding-left: 5px;
}

dd.time {
	width: auto;
	line-height: 200%;
	font-size: 1.1em;
}

dd.extra {
	width: 12%;
	line-height: 200%;
	text-align: center;
	font-size: 1.1em;
}

dd.mark {
	float: right !important;
	width: 9%;
	text-align: center;
	line-height: 200%;
	font-size: 1.2em;
}

dd.info {
	width: 30%;
}

dd.option {
	width: 15%;
	line-height: 200%;
	text-align: center;
	font-size: 1.1em;
}

dd.searchby {
	width: 47%;
	font-size: 1.1em;
	line-height: 1em;
}

ul.topiclist dd.searchextra {
	margin-left: 5px;
	padding: 0.2em 0;
	font-size: 1.1em;
	color: #333333;
	border-left: none;
	clear: both;
	width: 98%;
	overflow: hidden;
}

/* Container for post/reply buttons and pagination */
.topic-actions {
	margin-bottom: 3px;
	font-size: 1.1em;
	height: 28px;
	min-height: 28px;
}
div[class].topic-actions {
	height: auto;
}

/* Post body styles
----------------------------------------*/
.postbody {
	padding: 0;
	line-height: 1.48em;
	color: #333333;
	width: 76%;
	float: left;
	clear: both;
}

.postbody .ignore {
	font-size: 1.1em;
}

.postbody h3.first {
	/* The first post on the page uses this */
	font-size: 1.7em;
}

.postbody h3 {
	/* Postbody requires a different h3 format - so change it here */
	font-size: 1.5em;
	padding: 2px 0 0 0;
	margin: 0 0 0.3em 0 !important;
	text-transform: none;
	border: none;
	font-family: "Trebuchet MS", Verdana, Helvetica, Arial, sans-serif;
	line-height: 125%;
}

.postbody h3 img {
	/* Also see tweaks.css */
	vertical-align: bottom;
}

.postbody .content {
	font-size: 1.3em;
}

.search .postbody {
	width: 68%
}

/* Topic review panel
----------------------------------------*/
#review {
	margin-top: 2em;
}

#topicreview {
	padding-right: 5px;
	overflow: auto;
	height: 300px;
}

#topicreview .postbody {
	width: auto;
	float: none;
	margin: 0;
	height: auto;
}

#topicreview .post {
	height: auto;
}

#topicreview h2 {
	border-bottom-width: 0;
}

.post-ignore .postbody {
	display: none;
}

/* MCP Post details
----------------------------------------*/
#post_details
{
	/* This will only work in IE7+, plus the others */
	overflow: auto;
	max-height: 300px;
}

#expand
{
	clear: both;
}

/* Content container styles
----------------------------------------*/
.content {
	min-height: 3em;
	overflow: hidden;
	line-height: 1.4em;
	font-family: "Lucida Grande", "Trebuchet MS", Verdana, Helvetica, Arial, sans-serif;
	font-size: 1em;
	color: #333333;
	padding-bottom: 1px;
}

.content h2, .panel h2 {
	font-weight: normal;
	color: #989898;
	border-bottom: 1px solid #CCCCCC;
	font-size: 1.6em;
	margin-top: 0.5em;
	margin-bottom: 0.5em;
	padding-bottom: 0.5em;
}

.panel h3 {
	margin: 0.5em 0;
}

.panel p {
	font-size: 1.2em;
	margin-bottom: 1em;
	line-height: 1.4em;
}

.content p {
	font-family: "Lucida Grande", "Trebuchet MS", Verdana, Helvetica, Arial, sans-serif;
	font-size: 1.2em;
	margin-bottom: 1em;
	line-height: 1.4em;
}

dl.faq {
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	font-size: 1.1em;
	margin-top: 1em;
	margin-bottom: 2em;
	line-height: 1.4em;
}

dl.faq dt {
	font-weight: bold;
	color: #333333;
}

.content dl.faq {
	font-size: 1.2em;
	margin-bottom: 0.5em;
}

.content li {
	list-style-type: inherit;
}

.content ul, .content ol {
	margin-bottom: 1em;
	margin-left: 3em;
}

.posthilit {
	background-color: #f3f3f3;
	color: #BCBCBC;
	padding: 0 2px 1px 2px;
}

.announce, .unreadpost {
	/* Highlight the announcements & unread posts box */
	border-left-color: #BCBCBC;
	border-right-color: #BCBCBC;
}

/* Post author */
p.author {
	margin: 0 15em 0.6em 0;
	padding: 0 0 5px 0;
	font-family: Verdana, Helvetica, Arial, sans-serif;
	font-size: 1em;
	line-height: 1.2em;
}

/* Post signature */
.signature {
	margin-top: 1.5em;
	padding-top: 0.2em;
	font-size: 1.1em;
	border-top: 1px solid #CCCCCC;
	clear: left;
	line-height: 140%;
	overflow: hidden;
	width: 100%;
}

dd .signature {
	margin: 0;
	padding: 0;
	clear: none;
	border: none;
}

.signature li {
	list-style-type: inherit;
}

.signature ul, .signature ol {
	margin-bottom: 1em;
	margin-left: 3em;
}

/* Post noticies */
.notice {
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	width: auto;
	margin-top: 1.5em;
	padding-top: 0.2em;
	font-size: 1em;
	border-top: 1px dashed #CCCCCC;
	clear: left;
	line-height: 130%;
}

/* Jump to post link for now */
ul.searchresults {
	list-style: none;
	text-align: right;
	clear: both;
}

/* BB Code styles
----------------------------------------*/
/* Quote block */
blockquote {
	background: #ebebeb none 6px 8px no-repeat;
	border: 1px solid #dbdbdb;
	font-size: 0.95em;
	margin: 0.5em 1px 0 25px;
	overflow: hidden;
	padding: 5px;
}

blockquote blockquote {
	/* Nested quotes */
	background-color: #bababa;
	font-size: 1em;
	margin: 0.5em 1px 0 15px;	
}

blockquote blockquote blockquote {
	/* Nested quotes */
	background-color: #e4e4e4;
}

blockquote cite {
	/* Username/source of quoter */
	font-style: normal;
	font-weight: bold;
	margin-left: 20px;
	display: block;
	font-size: 0.9em;
}

blockquote cite cite {
	font-size: 1em;
}

blockquote.uncited {
	padding-top: 25px;
}

/* Code block */
dl.codebox {
	padding: 3px;
	background-color: #FFFFFF;
	border: 1px solid #d8d8d8;
	font-size: 1em;
}

dl.codebox dt {
	text-transform: uppercase;
	border-bottom: 1px solid #CCCCCC;
	margin-bottom: 3px;
	font-size: 0.8em;
	font-weight: bold;
	display: block;
}

blockquote dl.codebox {
	margin-left: 0;
}

dl.codebox code {
	/* Also see tweaks.css */
	overflow: auto;
	display: block;
	height: auto;
	max-height: 200px;
	white-space: normal;
	padding-top: 5px;
	font: 0.9em Monaco, "Andale Mono","Courier New", Courier, mono;
	line-height: 1.3em;
	color: #8b8b8b;
	margin: 2px 0;
}

.syntaxbg		{ color: #FFFFFF; }
.syntaxcomment	{ color: #000000; }
.syntaxdefault	{ color: #bcbcbc; }
.syntaxhtml		{ color: #000000; }
.syntaxkeyword	{ color: #585858; }
.syntaxstring	{ color: #a7a7a7; }

/* Attachments
----------------------------------------*/
.attachbox {
	float: left;
	width: auto; 
	margin: 5px 5px 5px 0;
	padding: 6px;
	background-color: #FFFFFF;
	border: 1px dashed #d8d8d8;
	clear: left;
}

.pm-message .attachbox {
	background-color: #f3f3f3;
}

.attachbox dt {
	font-family: Arial, Helvetica, sans-serif;
	text-transform: uppercase;
}

.attachbox dd {
	margin-top: 4px;
	padding-top: 4px;
	clear: left;
	border-top: 1px solid #d8d8d8;
}

.attachbox dd dd {
	border: none;
}

.attachbox p {
	line-height: 110%;
	color: #666666;
	font-weight: normal;
	clear: left;
}

.attachbox p.stats
{
	line-height: 110%;
	color: #666666;
	font-weight: normal;
	clear: left;
}

.attach-image {
	margin: 3px 0;
	width: 100%;
	max-height: 350px;
	overflow: auto;
}

.attach-image img {
	border: 1px solid #999999;
/*	cursor: move; */
	cursor: default;
}

/* Inline image thumbnails */
div.inline-attachment dl.thumbnail, div.inline-attachment dl.file {
	display: block;
	margin-bottom: 4px;
}

div.inline-attachment p {
	font-size: 100%;
}

dl.file {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	display: block;
}

dl.file dt {
	text-transform: none;
	margin: 0;
	padding: 0;
	font-weight: bold;
	font-family: Verdana, Arial, Helvetica, sans-serif;
}

dl.file dd {
	color: #666666;
	margin: 0;
	padding: 0;	
}

dl.thumbnail img {
	padding: 3px;
	border: 1px solid #666666;
	background-color: #FFF;
}

dl.thumbnail dd {
	color: #666666;
	font-style: italic;
	font-family: Verdana, Arial, Helvetica, sans-serif;
}

.attachbox dl.thumbnail dd {
	font-size: 100%;
}

dl.thumbnail dt a:hover {
	background-color: #EEEEEE;
}

dl.thumbnail dt a:hover img {
	border: 1px solid #d2d2d2;
}

/* Post poll styles
----------------------------------------*/
fieldset.polls {
	font-family: "Trebuchet MS", Verdana, Helvetica, Arial, sans-serif;
}

fieldset.polls dl {
	margin-top: 5px;
	border-top: 1px solid #e2e2e2;
	padding: 5px 0 0 0;
	line-height: 120%;
	color: #666666;
}

fieldset.polls dl.voted {
	font-weight: bold;
	color: #000000;
}

fieldset.polls dt {
	text-align: left;
	float: left;
	display: block;
	width: 30%;
	border-right: none;
	padding: 0;
	margin: 0;
	font-size: 1.1em;
}

fieldset.polls dd {
	float: left;
	width: 10%;
	border-left: none;
	padding: 0 5px;
	margin-left: 0;
	font-size: 1.1em;
}

fieldset.polls dd.resultbar {
	width: 50%;
}

fieldset.polls dd input {
	margin: 2px 0;
}

fieldset.polls dd div {
	text-align: right;
	font-family: Arial, Helvetica, sans-serif;
	color: #FFFFFF;
	font-weight: bold;
	padding: 0 2px;
	overflow: visible;
	min-width: 2%;
}

.pollbar1 {
	background-color: #aaaaaa;
	border-bottom: 1px solid #747474;
	border-right: 1px solid #747474;
}

.pollbar2 {
	background-color: #bebebe;
	border-bottom: 1px solid #8c8c8c;
	border-right: 1px solid #8c8c8c;
}

.pollbar3 {
	background-color: #D1D1D1;
	border-bottom: 1px solid #aaaaaa;
	border-right: 1px solid #aaaaaa;
}

.pollbar4 {
	background-color: #e4e4e4;
	border-bottom: 1px solid #bebebe;
	border-right: 1px solid #bebebe;
}

.pollbar5 {
	background-color: #f8f8f8;
	border-bottom: 1px solid #D1D1D1;
	border-right: 1px solid #D1D1D1;
}

/* Poster profile block
----------------------------------------*/
.postprofile {
	/* Also see tweaks.css */
	margin: 5px 0 10px 0;
	min-height: 80px;
	color: #666666;
	border-left: 1px solid #FFFFFF;
	width: 22%;
	float: right;
	display: inline;
}
.pm .postprofile {
	border-left: 1px solid #DDDDDD;
}

.postprofile dd, .postprofile dt {
	line-height: 1.2em;
	margin-left: 8px;
}

.postprofile strong {
	font-weight: normal;
	color: #000000;
}

.avatar {
	border: none;
	margin-bottom: 3px;
}

.online {
	background-image: none;
	background-position: 100% 0;
	background-repeat: no-repeat;
}

/* Poster profile used by search*/
.search .postprofile {
	width: 30%;
}

/* pm list in compose message if mass pm is enabled */
dl.pmlist dt {
	width: 60% !important;
}

dl.pmlist dt textarea {
	width: 95%;
}

dl.pmlist dd {
	margin-left: 61% !important;
	margin-bottom: 2px;
}
/* Button Styles
---------------------------------------- */

/* Rollover buttons
   Based on: http://wellstyled.com/css-nopreload-rollovers.html
----------------------------------------*/
.buttons {
	float: left;
	width: auto;
	height: auto;
}

/* Rollover state */
.buttons div {
	float: left;
	margin: 0 5px 0 0;
	background-position: 0 100%;
}

/* Rolloff state */
.buttons div a {
	display: block;
	width: 100%;
	height: 100%;
	background-position: 0 0;
	position: relative;
	overflow: hidden;
}

/* Hide <a> text and hide off-state image when rolling over (prevents flicker in IE) */
/*.buttons div span		{ display: none; }*/
/*.buttons div a:hover	{ background-image: none; }*/
.buttons div span			{ position: absolute; width: 100%; height: 100%; cursor: pointer;}
.buttons div a:hover span	{ background-position: 0 100%; }

/* Big button images */
.reply-icon span	{ background: transparent none 0 0 no-repeat; }
.post-icon span		{ background: transparent none 0 0 no-repeat; }
.locked-icon span	{ background: transparent none 0 0 no-repeat; }
.pmreply-icon span	{ background: none 0 0 no-repeat; }
.newpm-icon span 	{ background: none 0 0 no-repeat; }
.forwardpm-icon span 	{ background: none 0 0 no-repeat; }

/* Set big button dimensions */
.buttons div.reply-icon		{ width: 96px; height: 25px; }
.buttons div.post-icon		{ width: 96px; height: 25px; }
.buttons div.locked-icon	{ width: 88px; height: 25px; }
.buttons div.pmreply-icon	{ width: 96px; height: 25px; }
.buttons div.newpm-icon		{ width: 84px; height: 25px; }
.buttons div.forwardpm-icon	{ width: 96px; height: 25px; }

/* Sub-header (navigation bar)
--------------------------------------------- */
a.print, a.sendemail, a.fontsize {
	display: block;
	overflow: hidden;
	height: 18px;
	text-indent: -5000px;
	text-align: left;
	background-repeat: no-repeat;
}

a.print {
	background-image: none;
	width: 22px;
}

a.sendemail {
	background-image: none;
	width: 22px;
}

a.fontsize {
	background-image: none;
	background-position: 0 -1px;
	width: 29px;
}

a.fontsize:hover {
	background-position: 0 -20px;
	text-decoration: none;
}

/* Icon images
---------------------------------------- */
.sitehome, .icon-faq, .icon-members, .icon-home, .icon-ucp, .icon-register, .icon-logout,
.icon-bookmark, .icon-bump, .icon-subscribe, .icon-unsubscribe, .icon-pages, .icon-search {
	background-position: 0 50%;
	background-repeat: no-repeat;
	background-image: none;
	padding: 1px 0 0 17px;
}

/* Poster profile icons
----------------------------------------*/
ul.profile-icons {
	padding-top: 10px;
	list-style: none;
}

/* Rollover state */
ul.profile-icons li {
	float: left;
	margin: 0 6px 3px 0;
	background-position: 0 100%;
}

/* Rolloff state */
ul.profile-icons li a {
	display: block;
	width: 100%;
	height: 100%;
	background-position: 0 0;
}

/* Hide <a> text and hide off-state image when rolling over (prevents flicker in IE) */
ul.profile-icons li span { display:none; }
ul.profile-icons li a:hover { background: none; }

/* Positioning of moderator icons */
.postbody ul.profile-icons {
	float: right;
	width: auto;
	padding: 0;
}

.postbody ul.profile-icons li {
	margin: 0 3px;
}

/* Profile & navigation icons */
.email-icon, .email-icon a		{ background: none top left no-repeat; }
.aim-icon, .aim-icon a			{ background: none top left no-repeat; }
.yahoo-icon, .yahoo-icon a		{ background: none top left no-repeat; }
.web-icon, .web-icon a			{ background: none top left no-repeat; }
.thank-icon, .thank-icon a			{ background: none top left no-repeat; }
.msnm-icon, .msnm-icon a			{ background: none top left no-repeat; }
.icq-icon, .icq-icon a			{ background: none top left no-repeat; }
.jabber-icon, .jabber-icon a		{ background: none top left no-repeat; }
.pm-icon, .pm-icon a				{ background: none top left no-repeat; }
.quote-icon, .quote-icon a		{ background: none top left no-repeat; }

/* Moderator icons */
.report-icon, .report-icon a		{ background: none top left no-repeat; }
.warn-icon, .warn-icon a			{ background: none top left no-repeat; }
.edit-icon, .edit-icon a			{ background: none top left no-repeat; }
.delete-icon, .delete-icon a		{ background: none top left no-repeat; }
.info-icon, .info-icon a			{ background: none top left no-repeat; }

/* Set profile icon dimensions */
ul.profile-icons li.email-icon		{ width: 20px; height: 20px; }
ul.profile-icons li.aim-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.yahoo-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.web-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.thank-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.msnm-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.icq-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.jabber-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.pm-icon		{ width: 28px; height: 20px; }
ul.profile-icons li.quote-icon	{ width: 54px; height: 20px; }
ul.profile-icons li.report-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.edit-icon	{ width: 42px; height: 20px; }
ul.profile-icons li.delete-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.info-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.warn-icon	{ width: 20px; height: 20px; }

/* Fix profile icon default margins */
ul.profile-icons li.edit-icon	{ margin: 0 0 0 3px; }
ul.profile-icons li.quote-icon	{ margin: 0 0 0 10px; }
ul.profile-icons li.info-icon, ul.profile-icons li.report-icon	{ margin: 0 3px 0 0; }
/* Control Panel Styles
---------------------------------------- */


/* Main CP box
----------------------------------------*/
#cp-menu {
	float:left;
	width: 19%;
	margin-top: 1em;
	margin-bottom: 5px;
}

#cp-main {
	float: left;
	width: 81%;
}

#cp-main .content {
	padding: 0;
}

#cp-main h3, #cp-main hr, #cp-menu hr {
	border-color: #bfbfbf;
}

#cp-main .panel p {
	font-size: 1.1em;
}

#cp-main .panel ol {
	margin-left: 2em;
	font-size: 1.1em;
}

#cp-main .panel li.row {
	border-bottom: 1px solid #cbcbcb;
	border-top: 1px solid #F9F9F9;
}

ul.cplist {
	margin-bottom: 5px;
	border-top: 1px solid #cbcbcb;
}

#cp-main .panel li.header dd, #cp-main .panel li.header dt {
	color: #000000;
	margin-bottom: 2px;
}

#cp-main table.table1 {
	margin-bottom: 1em;
}

#cp-main table.table1 thead th {
	color: #333333;
	font-weight: bold;
	border-bottom: 1px solid #333333;
	padding: 5px;
}

#cp-main table.table1 tbody th {
	font-style: italic;
	background-color: transparent !important;
	border-bottom: none;
}

#cp-main .pagination {
	float: right;
	width: auto;
	padding-top: 1px;
}

#cp-main .postbody p {
	font-size: 1.1em;
}

#cp-main .pm-message {
	border: 1px solid #e2e2e2;
	margin: 10px 0;
	background-color: #FFFFFF;
	width: auto;
	float: none;
}

.pm-message h2 {
	padding-bottom: 5px;
}

#cp-main .postbody h3, #cp-main .box2 h3 {
	margin-top: 0;
}

#cp-main .buttons {
	margin-left: 0;
}

#cp-main ul.linklist {
	margin: 0;
}

/* MCP Specific tweaks */
.mcp-main .postbody {
	width: 100%;
}

/* CP tabbed menu
----------------------------------------*/
#tabs {
	line-height: normal;
	margin: 20px 0 -1px 7px;
	min-width: 570px;
}

#tabs ul {
	margin:0;
	padding: 0;
	list-style: none;
}

#tabs li {
	display: inline;
	margin: 0;
	padding: 0;
	font-size: 1em;
	font-weight: bold;
}

#tabs a {
	float: left;
	background: none no-repeat 0% -35px;
	margin: 0 1px 0 0;
	padding: 0 0 0 5px;
	text-decoration: none;
	position: relative;
	cursor: pointer;
}

#tabs a span {
	float: left;
	display: block;
	background: none no-repeat 100% -35px;
	padding: 6px 10px 6px 5px;
	color: #828282;
	white-space: nowrap;
}

#tabs a:hover span {
	color: #bcbcbc;
}

#tabs .activetab a {
	background-position: 0 0;
	border-bottom: 1px solid #ebebeb;
}

#tabs .activetab a span {
	background-position: 100% 0;
	padding-bottom: 7px;
	color: #333333;
}

#tabs a:hover {
	background-position: 0 -70px;
}

#tabs a:hover span {
	background-position:100% -70px;
}

#tabs .activetab a:hover {
	background-position: 0 0;
}

#tabs .activetab a:hover span {
	color: #000000;
	background-position: 100% 0;
}

/* Mini tabbed menu used in MCP
----------------------------------------*/
#minitabs {
	line-height: normal;
	margin: -20px 7px 0 0;
}

#minitabs ul {
	margin:0;
	padding: 0;
	list-style: none;
}

#minitabs li {
	display: block;
	float: right;
	padding: 0 10px 4px 10px;
	font-size: 1em;
	font-weight: bold;
	background-color: #f2f2f2;
	margin-left: 2px;
}

#minitabs a {
}

#minitabs a:hover {
	text-decoration: none;
}

#minitabs li.activetab {
	background-color: #F9F9F9;
}

#minitabs li.activetab a, #minitabs li.activetab a:hover {
	color: #333333;
}

/* UCP navigation menu
----------------------------------------*/
/* Container for sub-navigation list */
#navigation {
	width: 100%;
	padding-top: 36px;
}

#navigation ul {
	list-style:none;
}

/* Default list state */
#navigation li {
	margin: 1px 0;
	padding: 0;
	font-weight: bold;
	display: inline;
}

/* Link styles for the sub-section links */
#navigation a {
	display: block;
	padding: 5px;
	margin: 1px 0;
	text-decoration: none;
	font-weight: bold;
	color: #333;
	background: #cfcfcf none repeat-y 100% 0;
}

#navigation a:hover {
	text-decoration: none;
	background-color: #c6c6c6;
	color: #bcbcbc;
	background-image: none;
}

#navigation #active-subsection a {
	display: block;
	color: #d3d3d3;
	background-color: #F9F9F9;
	background-image: none;
}

#navigation #active-subsection a:hover {
	color: #d3d3d3;
}

/* Preferences pane layout
----------------------------------------*/
#cp-main h2 {
	border-bottom: none;
	padding: 0;
	margin-left: 10px;
	color: #333333;
}

#cp-main .panel {
	background-color: #F9F9F9;
}

#cp-main .pm {
	background-color: #FFFFFF;
}

#cp-main span.corners-top, #cp-menu span.corners-top {
	background-image: none;
}

#cp-main span.corners-top span, #cp-menu span.corners-top span {
	background-image: none;
}

#cp-main span.corners-bottom, #cp-menu span.corners-bottom {
	background-image: none;
}

#cp-main span.corners-bottom span, #cp-menu span.corners-bottom span {
	background-image: none;
}

/* Topicreview */
#cp-main .panel #topicreview span.corners-top, #cp-menu .panel #topicreview span.corners-top {
	background-image: none;
}

#cp-main .panel #topicreview span.corners-top span, #cp-menu .panel #topicreview span.corners-top span {
	background-image: none;
}

#cp-main .panel #topicreview span.corners-bottom, #cp-menu .panel #topicreview span.corners-bottom {
	background-image: none;
}

#cp-main .panel #topicreview span.corners-bottom span, #cp-menu .panel #topicreview span.corners-bottom span {
	background-image: none;
}

/* Friends list */
.cp-mini {
	background-color: #f9f9f9;
	padding: 0 5px;
	margin: 10px 15px 10px 5px;
}

.cp-mini span.corners-top, .cp-mini span.corners-bottom {
	margin: 0 -5px;
}

dl.mini dt {
	font-weight: bold;
	color: #676767;
}

dl.mini dd {
	padding-top: 4px;
}

.friend-online {
	font-weight: bold;
}

.friend-offline {
	font-style: italic;
}

/* PM Styles
----------------------------------------*/
#pm-menu {
	line-height: 2.5em;
}

/* PM panel adjustments */
.pm-panel-header {
	margin: 0; 
	padding-bottom: 10px; 
	border-bottom: 1px dashed #A4B3BF;
}

.reply-all {
	display: block; 
	padding-top: 4px; 
	clear: both;
	float: left;
}

.pm-panel-message {
	padding-top: 10px;
}

.pm-return-to {
	padding-top: 23px;
}

#cp-main .pm-message-nav {
	margin: 0; 
	padding: 2px 10px 5px 10px; 
	border-bottom: 1px dashed #A4B3BF;
}

/* PM Message history */
.current {
	color: #999999;
}

/* Defined rules list for PM options */
ol.def-rules {
	padding-left: 0;
}

ol.def-rules li {
	line-height: 180%;
	padding: 1px;
}

/* PM marking colours */
.pmlist li.bg1 {
	padding: 0 3px;
}

.pmlist li.bg2 {
	padding: 0 3px;
}

.pmlist li.pm_message_reported_colour, .pm_message_reported_colour {
	border-left-color: #bcbcbc;
	border-right-color: #bcbcbc;
}

.pmlist li.pm_marked_colour, .pm_marked_colour {
	padding: 0;
	border: solid 3px #ffffff;
	border-width: 0 3px;
}

.pmlist li.pm_replied_colour, .pm_replied_colour {
	padding: 0;
	border: solid 3px #c2c2c2;
	border-width: 0 3px;
}

.pmlist li.pm_friend_colour, .pm_friend_colour {
	padding: 0;
	border: solid 3px #bdbdbd;
	border-width: 0 3px;
}

.pmlist li.pm_foe_colour, .pm_foe_colour {
	padding: 0;
	border: solid 3px #000000;
	border-width: 0 3px;
}

.pm-legend {
	border-left-width: 10px;
	border-left-style: solid;
	border-right-width: 0;
	margin-bottom: 3px;
	padding-left: 3px;
}

/* Avatar gallery */
#gallery label {
	position: relative;
	float: left;
	margin: 10px;
	padding: 5px;
	width: auto;
	background: #FFFFFF;
	border: 1px solid #CCC;
	text-align: center;
}

#gallery label:hover {
	background-color: #EEE;
}
/* Form Styles
---------------------------------------- */

/* General form styles
----------------------------------------*/
fieldset {
	border-width: 0;
	font-family: Verdana, Helvetica, Arial, sans-serif;
	font-size: 1.1em;
}

input {
	font-weight: normal;
	cursor: pointer;
	vertical-align: middle;
	padding: 0 3px;
	font-size: 1em;
	font-family: Verdana, Helvetica, Arial, sans-serif;
}

select {
	font-family: Verdana, Helvetica, Arial, sans-serif;
	font-weight: normal;
	cursor: pointer;
	vertical-align: middle;
	border: 1px solid #666666;
	padding: 1px;
	background-color: #FAFAFA;
	font-size: 1em;
}

option {
	padding-right: 1em;
}

option.disabled-option {
	color: graytext;
}

textarea {
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	width: 60%;
	padding: 2px;
	font-size: 1em;
	line-height: 1.4em;
}

label {
	cursor: default;
	padding-right: 5px;
	color: #676767;
}

label input {
	vertical-align: middle;
}

label img {
	vertical-align: middle;
}

/* Definition list layout for forms
---------------------------------------- */
fieldset dl {
	padding: 4px 0;
}

fieldset dt {
	float: left;	
	width: 40%;
	text-align: left;
	display: block;
}

fieldset dd {
	margin-left: 41%;
	vertical-align: top;
	margin-bottom: 3px;
}

/* Specific layout 1 */
fieldset.fields1 dt {
	width: 15em;
	border-right-width: 0;
}

fieldset.fields1 dd {
	margin-left: 15em;
	border-left-width: 0;
}

fieldset.fields1 {
	background-color: transparent;
}

fieldset.fields1 div {
	margin-bottom: 3px;
}

/* Set it back to 0px for the reCaptcha divs: PHPBB3-9587 */
fieldset.fields1 #recaptcha_widget_div div {
	margin-bottom: 0;
}

/* Specific layout 2 */
fieldset.fields2 dt {
	width: 15em;
	border-right-width: 0;
}

fieldset.fields2 dd {
	margin-left: 16em;
	border-left-width: 0;
}

/* Form elements */
dt label {
	font-weight: bold;
	text-align: left;
}

dd label {
	white-space: nowrap;
	color: #333;
}

dd input, dd textarea {
	margin-right: 3px;
}

dd select {
	width: auto;
}

dd textarea {
	width: 85%;
}

/* Hover effects */
fieldset dl:hover dt label {
	color: #000000;
}

fieldset.fields2 dl:hover dt label {
	color: inherit;
}

#timezone {
	width: 95%;
}

* html #timezone {
	width: 50%;
}

/* Quick-login on index page */
fieldset.quick-login {
	margin-top: 5px;
}

fieldset.quick-login input {
	width: auto;
}

fieldset.quick-login input.inputbox {
	width: 15%;
	vertical-align: middle;
	margin-right: 5px;
	background-color: #f3f3f3;
}

fieldset.quick-login label {
	white-space: nowrap;
	padding-right: 2px;
}

/* Display options on viewtopic/viewforum pages  */
fieldset.display-options {
	text-align: center;
	margin: 3px 0 5px 0;
}

fieldset.display-options label {
	white-space: nowrap;
	padding-right: 2px;
}

fieldset.display-options a {
	margin-top: 3px;
}

/* Display actions for ucp and mcp pages */
fieldset.display-actions {
	text-align: right;
	line-height: 2em;
	white-space: nowrap;
	padding-right: 1em;
}

fieldset.display-actions label {
	white-space: nowrap;
	padding-right: 2px;
}

fieldset.sort-options {
	line-height: 2em;
}

/* MCP forum selection*/
fieldset.forum-selection {
	margin: 5px 0 3px 0;
	float: right;
}

fieldset.forum-selection2 {
	margin: 13px 0 3px 0;
	float: right;
}

/* Jumpbox */
fieldset.jumpbox {
	text-align: right;
	margin-top: 15px;
	height: 2.5em;
}

fieldset.quickmod {
	width: 50%;
	float: right;
	text-align: right;
	height: 2.5em;
}

/* Submit button fieldset */
fieldset.submit-buttons {
	text-align: center;
	vertical-align: middle;
	margin: 5px 0;
}

fieldset.submit-buttons input {
	vertical-align: middle;
	padding-top: 3px;
	padding-bottom: 3px;
}

/* Posting page styles
----------------------------------------*/

/* Buttons used in the editor */
#format-buttons {
	margin: 15px 0 2px 0;
}

#format-buttons input, #format-buttons select {
	vertical-align: middle;
}

/* Main message box */
#message-box {
	width: 80%;
}

#message-box textarea {
	font-family: "Trebuchet MS", Verdana, Helvetica, Arial, sans-serif;
	width: 450px;
	height: 270px;
	min-width: 100%;
	max-width: 100%;
	font-size: 1.2em;
	color: #333333;
}

/* Emoticons panel */
#smiley-box {
	width: 18%;
	float: right;
}

#smiley-box img {
	margin: 3px;
}

/* Input field styles
---------------------------------------- */
.inputbox {
	background-color: #FFFFFF;
	border: 1px solid #c0c0c0;
	color: #333333;
	padding: 2px;
	cursor: text;
}

.inputbox:hover {
	border: 1px solid #eaeaea;
}

.inputbox:focus {
	border: 1px solid #eaeaea;
	color: #4b4b4b;
}

input.inputbox	{ width: 85%; }
input.medium	{ width: 50%; }
input.narrow	{ width: 25%; }
input.tiny		{ width: 125px; }

textarea.inputbox {
	width: 85%;
}

.autowidth {
	width: auto !important;
}

/* Form button styles
---------------------------------------- */
input.button1, input.button2 {
	font-size: 1em;
}

a.button1, input.button1, input.button3, a.button2, input.button2 {
	width: auto !important;
	padding-top: 1px;
	padding-bottom: 1px;
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	color: #000;
	background: #FAFAFA none repeat-x top left;
}

a.button1, input.button1 {
	font-weight: bold;
	border: 1px solid #666666;
}

input.button3 {
	padding: 0;
	margin: 0;
	line-height: 5px;
	height: 12px;
	background-image: none;
	font-variant: small-caps;
}

/* Alternative button */
a.button2, input.button2, input.button3 {
	border: 1px solid #666666;
}

/* <a> button in the style of the form buttons */
a.button1, a.button1:link, a.button1:visited, a.button1:active, a.button2, a.button2:link, a.button2:visited, a.button2:active {
	text-decoration: none;
	color: #000000;
	padding: 2px 8px;
	line-height: 250%;
	vertical-align: text-bottom;
	background-position: 0 1px;
}

/* Hover states */
a.button1:hover, input.button1:hover, a.button2:hover, input.button2:hover, input.button3:hover {
	border: 1px solid #BCBCBC;
	background-position: 0 100%;
	color: #BCBCBC;
}

input.disabled {
	font-weight: normal;
	color: #666666;
}

/* Topic and forum Search */
.search-box {
	margin-top: 3px;
	margin-left: 5px;
	float: left;
}

.search-box input {
}

input.search {
	background-image: none;
	background-repeat: no-repeat;
	background-position: left 1px;
	padding-left: 17px;
}

.full { width: 95%; }
.medium { width: 50%;}
.narrow { width: 25%;}
.tiny { width: 10%;}
/* Style Sheet Tweaks

These style definitions are mainly IE specific 
tweaks required due to its poor CSS support.
-------------------------------------------------*/

* html table, * html select, * html input { font-size: 100%; }
* html hr { margin: 0; }
* html span.corners-top, * html span.corners-bottom { background-image: url("./styles/prosilver/theme/images/corners_left.gif"); }
* html span.corners-top span, * html span.corners-bottom span { background-image: url("./styles/prosilver/theme/images/corners_right.gif"); }

table.table1 {
	width: 99%;		/* IE < 6 browsers */
	/* Tantek hack */
	voice-family: "\"}\"";
	voice-family: inherit;
	width: 100%;
}
html>body table.table1 { width: 100%; }	/* Reset 100% for opera */

* html ul.topiclist li { position: relative; }
* html .postbody h3 img { vertical-align: middle; }

/* Form styles */
html>body dd label input { vertical-align: text-bottom; }	/* Align checkboxes/radio buttons nicely */

* html input.button1, * html input.button2 {
	padding-bottom: 0;
	margin-bottom: 1px;
}

/* Misc layout styles */
* html .column1, * html .column2 { width: 45%; }

/* Nice method for clearing floated blocks without having to insert any extra markup (like spacer above)
   From http://www.positioniseverything.net/easyclearing.html 
#tabs:after, #minitabs:after, .post:after, .navbar:after, fieldset dl:after, ul.topiclist dl:after, ul.linklist:after, dl.polls:after {
	content: "."; 
	display: block; 
	height: 0; 
	clear: both; 
	visibility: hidden;
}*/

.clearfix, #tabs, #minitabs, fieldset dl, ul.topiclist dl, dl.polls {
	height: 1%;
	overflow: hidden;
}

/* viewtopic fix */
* html .post {
	height: 25%;
	overflow: hidden;
}

/* navbar fix */
* html .clearfix, * html .navbar, ul.linklist {
	height: 4%;
	overflow: hidden;
}

/* Simple fix so forum and topic lists always have a min-height set, even in IE6
	From http://www.dustindiaz.com/min-height-fast-hack */
dl.icon {
	min-height: 35px;
	height: auto !important;
	height: 35px;
}

* html li.row dl.icon dt {
	height: 35px;
	overflow: visible;
}

* html #search-box {
	width: 25%;
}

/* Correctly clear floating for details on profile view */
*:first-child+html dl.details dd {
	margin-left: 30%;
	float: none;
}

* html dl.details dd {
	margin-left: 30%;
	float: none;
}

* html .forumbg table.table1 {
	margin: 0 -2px 0px -1px;
}

/* Headerbar height fix for IE7 and below */
* html #site-description p {
	margin-bottom: 1.0em;
}

*:first-child+html #site-description p {
	margin-bottom: 1.0em;
}
/*  	
--------------------------------------------------------------
Colours and backgrounds for common.css
-------------------------------------------------------------- */

html, body {
	color: #536482;
}

h1 {
	color: #FFFFFF;
}

h2 {
	color: #28313F;
}

h3 {
	border-bottom-color: #CCCCCC;
	color: #115098;
}

hr {
	border-color: #FFFFFF;
	border-top-color: #CCCCCC;
}

hr.dashed {
	border-top-color: #CCCCCC;
}

/* Search box
--------------------------------------------- */

#search-box {
	color: #FFFFFF;
}

#search-box #keywords {
	background-color: #FFF;
}

#search-box input {
	border-color: #0075B0;
}

/* Round cornered boxes and backgrounds
---------------------------------------- */
.headerbar {
	background-color: #12A3EB;
	background-image: url("./styles/prosilver/theme/images/bg_header.gif");
	color: #FFFFFF;
}

.navbar {
	background-color: #cadceb;
}

.forabg {
	background-color: #bbcde0;
	background-image: url("./styles/prosilver/theme/images/bg_list.gif");
}

.forumbg {
	background-color: #abbfd4;
	background-image: url("./styles/prosilver/theme/images/bg_header.gif");
}

.panel {
	background-color: #ECF1F3;
	color: #28313F;
}

.post:target .content {
	color: #000000;
}

.post:target h3 a {
	color: #000000;
}

.bg1	{ background-color: #ECF3F7; }
.bg2	{ background-color: #e1ebf2;  }
.bg3	{ background-color: #cadceb; }

.ucprowbg {
	background-color: #DCDEE2;
}

.fieldsbg {
	background-color: #E7E8EA;
}

span.corners-top {
	background-image: url("./styles/prosilver/theme/images/corners_left.png");
}

span.corners-top span {
	background-image: url("./styles/prosilver/theme/images/corners_right.png");
}

span.corners-bottom {
	background-image: url("./styles/prosilver/theme/images/corners_left.png");
}

span.corners-bottom span {
	background-image: url("./styles/prosilver/theme/images/corners_right.png");
}

/* Horizontal lists
----------------------------------------*/

ul.navlinks {
	border-bottom-color: #FFFFFF;
}

/* Table styles
----------------------------------------*/
table.table1 thead th {
	color: #FFFFFF;
}

table.table1 tbody tr {
	border-color: #BFC1CF;
}

table.table1 tbody tr:hover, table.table1 tbody tr.hover {
	background-color: #CFE1F6;
	color: #000;
}

table.table1 td {
	color: #536482;
}

table.table1 tbody td {
	border-top-color: #FAFAFA;
}

table.table1 tbody th {
	border-bottom-color: #000000;
	color: #333333;
	background-color: #FFFFFF;
}

table.info tbody th {
	color: #000000;
}

/* Misc layout styles
---------------------------------------- */
dl.details dt {
	color: #000000;
}

dl.details dd {
	color: #536482;
}

.sep {
	color: #1198D9;
}

/* Pagination
---------------------------------------- */

.pagination span strong {
	color: #FFFFFF;
	background-color: #4692BF;
	border-color: #4692BF;
}

.pagination span a, .pagination span a:link, .pagination span a:visited {
	color: #5C758C;
	background-color: #ECEDEE;
	border-color: #B4BAC0;
}

.pagination span a:hover {
	border-color: #368AD2;
	background-color: #368AD2;
	color: #FFF;
}

.pagination span a:active {
	color: #5C758C;
	background-color: #ECEDEE;
	border-color: #B4BAC0;
}

/* Pagination in viewforum for multipage topics */
.row .pagination {
	background-image: url("./styles/prosilver/theme/images/icon_pages.gif");
}

.row .pagination span a, li.pagination span a {
	background-color: #FFFFFF;
}

.row .pagination span a:hover, li.pagination span a:hover {
	background-color: #368AD2;
}

/* Miscellaneous styles
---------------------------------------- */

.copyright {
	color: #555555;
}

.error {
	color: #BC2A4D;
}

.reported {
	background-color: #F7ECEF;
}

li.reported:hover {
	background-color: #ECD5D8 !important;
}
.sticky, .announce {
	/* you can add a background for stickies and announcements*/
}

div.rules {
	background-color: #ECD5D8;
	color: #BC2A4D;
}

p.rules {
	background-color: #ECD5D8;
	background-image: none;
}

/*  	
--------------------------------------------------------------
Colours and backgrounds for links.css
-------------------------------------------------------------- */

a:link	{ color: #105289; }
a:visited	{ color: #105289; }
a:hover	{ color: #D31141; }
a:active	{ color: #368AD2; }

/* Links on gradient backgrounds */
#search-box a:link, .navbg a:link, .forumbg .header a:link, .forabg .header a:link, th a:link {
	color: #FFFFFF;
}

#search-box a:visited, .navbg a:visited, .forumbg .header a:visited, .forabg .header a:visited, th a:visited {
	color: #FFFFFF;
}

#search-box a:hover, .navbg a:hover, .forumbg .header a:hover, .forabg .header a:hover, th a:hover {
	color: #A8D8FF;
}

#search-box a:active, .navbg a:active, .forumbg .header a:active, .forabg .header a:active, th a:active {
	color: #C8E6FF;
}

/* Links for forum/topic lists */
a.forumtitle {
	color: #105289;
}

/* a.forumtitle:visited { color: #105289; } */

a.forumtitle:hover {
	color: #BC2A4D;
}

a.forumtitle:active {
	color: #105289;
}

a.topictitle {
	color: #105289;
}

/* a.topictitle:visited { color: #368AD2; } */

a.topictitle:hover {
	color: #BC2A4D;
}

a.topictitle:active {
	color: #105289;
}

/* Post body links */
.postlink {
	color: #368AD2;
	border-bottom-color: #368AD2;
}

.postlink:visited {
	color: #5D8FBD;
	border-bottom-color: #5D8FBD;
}

.postlink:active {
	color: #368AD2;
}

.postlink:hover {
	background-color: #D0E4F6;
	color: #0D4473;
}

.signature a, .signature a:visited, .signature a:hover, .signature a:active {
	background-color: transparent;
}

/* Profile links */
.postprofile a:link, .postprofile a:visited, .postprofile dt.author a {
	color: #105289;
}

.postprofile a:hover, .postprofile dt.author a:hover {
	color: #D31141;
}

.postprofile a:active {
	color: #105289;
}

/* Profile searchresults */	
.search .postprofile a {
	color: #105289;
}

.search .postprofile a:hover {
	color: #D31141;
}

/* Back to top of page */
a.top {
	background-image: url("./styles/prosilver/imageset/icon_back_top.gif");
}

a.top2 {
	background-image: url("./styles/prosilver/imageset/icon_back_top.gif");
}

/* Arrow links  */
a.up		{ background-image: url("./styles/prosilver/theme/images/arrow_up.gif") }
a.down		{ background-image: url("./styles/prosilver/theme/images/arrow_down.gif") }
a.left		{ background-image: url("./styles/prosilver/theme/images/arrow_left.gif") }
a.right		{ background-image: url("./styles/prosilver/theme/images/arrow_right.gif") }

a.up:hover {
	background-color: transparent;
}

a.left:hover {
	color: #368AD2;
}

a.right:hover {
	color: #368AD2;
}


/*  	
--------------------------------------------------------------
Colours and backgrounds for content.css
-------------------------------------------------------------- */

ul.forums {
	background-color: #eef5f9;
	background-image: url("./styles/prosilver/theme/images/gradient.gif");
}

ul.topiclist li {
	color: #4C5D77;
}

ul.topiclist dd {
	border-left-color: #FFFFFF;
}

.rtl ul.topiclist dd {
	border-right-color: #fff;
	border-left-color: transparent;
}

ul.topiclist li.row dt a.subforum.read {
	background-image: url("./styles/prosilver/imageset/subforum_read.gif");
}

ul.topiclist li.row dt a.subforum.unread {
	background-image: url("./styles/prosilver/imageset/subforum_unread.gif");
}

li.row {
	border-top-color:  #FFFFFF;
	border-bottom-color: #00608F;
}

li.row strong {
	color: #000000;
}

li.row:hover {
	background-color: #F6F4D0;
}

li.row:hover dd {
	border-left-color: #CCCCCC;
}

.rtl li.row:hover dd {
	border-right-color: #CCCCCC;
	border-left-color: transparent;
}

li.header dt, li.header dd {
	color: #FFFFFF;
}

/* Forum list column styles */
ul.topiclist dd.searchextra {
	color: #333333;
}

/* Post body styles
----------------------------------------*/
.postbody {
	color: #333333;
}

/* Content container styles
----------------------------------------*/
.content {
	color: #333333;
}

.content h2, .panel h2 {
	color: #115098;
	border-bottom-color:  #CCCCCC;
}

dl.faq dt {
	color: #333333;
}

.posthilit {
	background-color: #F3BFCC;
	color: #BC2A4D;
}

/* Post signature */
.signature {
	border-top-color: #CCCCCC;
}

/* Post noticies */
.notice {
	border-top-color:  #CCCCCC;
}

/* BB Code styles
----------------------------------------*/
/* Quote block */
blockquote {
	background-color: #EBEADD;
	background-image: url("./styles/prosilver/theme/images/quote.gif");
	border-color:#DBDBCE;
}

.rtl blockquote {
	background-image: url("./styles/prosilver/theme/images/quote_rtl.gif");
}

blockquote blockquote {
	/* Nested quotes */
	background-color:#EFEED9;
}

blockquote blockquote blockquote {
	/* Nested quotes */
	background-color: #EBEADD;
}

/* Code block */
dl.codebox {
	background-color: #FFFFFF;
	border-color: #C9D2D8;
}

dl.codebox dt {
	border-bottom-color:  #CCCCCC;
}

dl.codebox code {
	color: #2E8B57;
}

.syntaxbg		{ color: #FFFFFF; }
.syntaxcomment	{ color: #FF8000; }
.syntaxdefault	{ color: #0000BB; }
.syntaxhtml		{ color: #000000; }
.syntaxkeyword	{ color: #007700; }
.syntaxstring	{ color: #DD0000; }

/* Attachments
----------------------------------------*/
.attachbox {
	background-color: #FFFFFF;
	border-color:  #C9D2D8;
}

.pm-message .attachbox {
	background-color: #F2F3F3;
}

.attachbox dd {
	border-top-color: #C9D2D8;
}

.attachbox p {
	color: #666666;
}

.attachbox p.stats {
	color: #666666;
}

.attach-image img {
	border-color: #999999;
}

/* Inline image thumbnails */

dl.file dd {
	color: #666666;
}

dl.thumbnail img {
	border-color: #666666;
	background-color: #FFFFFF;
}

dl.thumbnail dd {
	color: #666666;
}

dl.thumbnail dt a:hover {
	background-color: #EEEEEE;
}

dl.thumbnail dt a:hover img {
	border-color: #368AD2;
}

/* Post poll styles
----------------------------------------*/

fieldset.polls dl {
	border-top-color: #DCDEE2;
	color: #666666;
}

fieldset.polls dl.voted {
	color: #000000;
}

fieldset.polls dd div {
	color: #FFFFFF;
}

.rtl .pollbar1, .rtl .pollbar2, .rtl .pollbar3, .rtl .pollbar4, .rtl .pollbar5 {
	border-right-color: transparent;
}

.pollbar1 {
	background-color: #AA2346;
	border-bottom-color: #74162C;
	border-right-color: #74162C;
}

.rtl .pollbar1 {
	border-left-color: #74162C;
}

.pollbar2 {
	background-color: #BE1E4A;
	border-bottom-color: #8C1C38;
	border-right-color: #8C1C38;
}

.rtl .pollbar2 {
	border-left-color: #8C1C38;
}

.pollbar3 {
	background-color: #D11A4E;
	border-bottom-color: #AA2346;
	border-right-color: #AA2346;
}

.rtl .pollbar3 {
	border-left-color: #AA2346;
}

.pollbar4 {
	background-color: #E41653;
	border-bottom-color: #BE1E4A;
	border-right-color: #BE1E4A;
}

.rtl .pollbar4 {
	border-left-color: #BE1E4A;
}

.pollbar5 {
	background-color: #F81157;
	border-bottom-color: #D11A4E;
	border-right-color: #D11A4E;
}

.rtl .pollbar5 {
	border-left-color: #D11A4E;
}

/* Poster profile block
----------------------------------------*/
.postprofile {
	color: #666666;
	border-left-color: #FFFFFF;
}

.rtl .postprofile {
	border-right-color: #FFFFFF;
	border-left-color: transparent;
}

.pm .postprofile {
	border-left-color: #DDDDDD;
}

.rtl .pm .postprofile {
	border-right-color: #DDDDDD;
	border-left-color: transparent;
}

.postprofile strong {
	color: #000000;
}

.online {
	background-image: url("./styles/prosilver/imageset/en/icon_user_online.gif");
}

/*  	
--------------------------------------------------------------
Colours and backgrounds for buttons.css
-------------------------------------------------------------- */

/* Big button images */
.reply-icon span	{ background-image: url("./styles/prosilver/imageset/en/button_topic_reply.gif"); }
.post-icon span		{ background-image: url("./styles/prosilver/imageset/en/button_topic_new.gif"); }
.locked-icon span	{ background-image: url("./styles/prosilver/imageset/en/button_topic_locked.gif"); }
.pmreply-icon span	{ background-image: url("./styles/prosilver/imageset/en/button_pm_reply.gif") ;}
.newpm-icon span 	{ background-image: url("./styles/prosilver/imageset/en/button_pm_new.gif") ;}
.forwardpm-icon span	{ background-image: url("./styles/prosilver/imageset/en/button_pm_forward.gif") ;}

a.print {
	background-image: url("./styles/prosilver/theme/images/icon_print.gif");
}

a.sendemail {
	background-image: url("./styles/prosilver/theme/images/icon_sendemail.gif");
}

a.fontsize {
	background-image: url("./styles/prosilver/theme/images/icon_fontsize.gif");
}

/* Icon images
---------------------------------------- */
.sitehome						{ background-image: url("./styles/prosilver/theme/images/icon_home.gif"); }
.icon-faq						{ background-image: url("./styles/prosilver/theme/images/icon_faq.gif"); }
.icon-members					{ background-image: url("./styles/prosilver/theme/images/icon_members.gif"); }
.icon-home						{ background-image: url("./styles/prosilver/theme/images/icon_home.gif"); }
.icon-ucp						{ background-image: url("./styles/prosilver/theme/images/icon_ucp.gif"); }
.icon-register					{ background-image: url("./styles/prosilver/theme/images/icon_register.gif"); }
.icon-logout					{ background-image: url("./styles/prosilver/theme/images/icon_logout.gif"); }
.icon-bookmark					{ background-image: url("./styles/prosilver/theme/images/icon_bookmark.gif"); }
.icon-bump						{ background-image: url("./styles/prosilver/theme/images/icon_bump.gif"); }
.icon-subscribe					{ background-image: url("./styles/prosilver/theme/images/icon_subscribe.gif"); }
.icon-unsubscribe				{ background-image: url("./styles/prosilver/theme/images/icon_unsubscribe.gif"); }
.icon-pages						{ background-image: url("./styles/prosilver/theme/images/icon_pages.gif"); }
.icon-search					{ background-image: url("./styles/prosilver/theme/images/icon_search.gif"); }

/* Profile & navigation icons */
.email-icon, .email-icon a		{ background-image: url("./styles/prosilver/imageset/icon_contact_email.gif"); }
.aim-icon, .aim-icon a			{ background-image: url("./styles/prosilver/imageset/icon_contact_aim.gif"); }
.yahoo-icon, .yahoo-icon a		{ background-image: url("./styles/prosilver/imageset/icon_contact_yahoo.gif"); }
.web-icon, .web-icon a			{ background-image: url("./styles/prosilver/imageset/icon_contact_www.gif"); }
.msnm-icon, .msnm-icon a			{ background-image: url("./styles/prosilver/imageset/icon_contact_msnm.gif"); }
.icq-icon, .icq-icon a			{ background-image: url("./styles/prosilver/imageset/icon_contact_icq.gif"); }
.jabber-icon, .jabber-icon a		{ background-image: url("./styles/prosilver/imageset/icon_contact_jabber.gif"); }
.pm-icon, .pm-icon a				{ background-image: url("./styles/prosilver/imageset/en/icon_contact_pm.gif"); }
.quote-icon, .quote-icon a		{ background-image: url("./styles/prosilver/imageset/en/icon_post_quote.gif"); }

/* Moderator icons */
.report-icon, .report-icon a		{ background-image: url("./styles/prosilver/imageset/icon_post_report.gif"); }
.edit-icon, .edit-icon a			{ background-image: url("./styles/prosilver/imageset/en/icon_post_edit.gif"); }
.delete-icon, .delete-icon a		{ background-image: url("./styles/prosilver/imageset/icon_post_delete.gif"); }
.info-icon, .info-icon a			{ background-image: url("./styles/prosilver/imageset/icon_post_info.gif"); }
.warn-icon, .warn-icon a			{ background-image: url("./styles/prosilver/imageset/icon_user_warn.gif"); } /* Need updated warn icon */

/*  	
--------------------------------------------------------------
Colours and backgrounds for cp.css
-------------------------------------------------------------- */

/* Main CP box
----------------------------------------*/

#cp-main h3, #cp-main hr, #cp-menu hr {
	border-color: #A4B3BF;
}

#cp-main .panel li.row {
	border-bottom-color: #B5C1CB;
	border-top-color: #F9F9F9;
}

ul.cplist {
	border-top-color: #B5C1CB;
}

#cp-main .panel li.header dd, #cp-main .panel li.header dt {
	color: #000000;
}

#cp-main table.table1 thead th {
	color: #333333;
	border-bottom-color: #333333;
}

#cp-main .pm-message {
	border-color: #DBDEE2;
	background-color: #FFFFFF;
}

/* CP tabbed menu
----------------------------------------*/
#tabs a {
	background-image: url("./styles/prosilver/theme/images/bg_tabs1.gif");
}

#tabs a span {
	background-image: url("./styles/prosilver/theme/images/bg_tabs2.gif");
	color: #536482;
}

#tabs a:hover span {
	color: #BC2A4D;
}

#tabs .activetab a {
	border-bottom-color: #CADCEB;
}

#tabs .activetab a span {
	color: #333333;
}

#tabs .activetab a:hover span {
	color: #000000;
}

/* Mini tabbed menu used in MCP
----------------------------------------*/
#minitabs li {
	background-color: #E1EBF2;
}

#minitabs li.activetab {
	background-color: #F9F9F9;
}

#minitabs li.activetab a, #minitabs li.activetab a:hover {
	color: #333333;
}

/* UCP navigation menu
----------------------------------------*/

/* Link styles for the sub-section links */
#navigation a {
	color: #333;
	background-color: #B2C2CF;
	background-image: url("./styles/prosilver/theme/images/bg_menu.gif");
}

.rtl #navigation a {
	background-image: url("./styles/prosilver/theme/images/bg_menu_rtl.gif");
	background-position: 0 100%;
}

#navigation a:hover {
	background-image: none;
	background-color: #aabac6;
	color: #BC2A4D;
}

#navigation #active-subsection a {
	color: #D31141;
	background-color: #F9F9F9;
	background-image: none;
}

#navigation #active-subsection a:hover {
	color: #D31141;
}

/* Preferences pane layout
----------------------------------------*/
#cp-main h2 {
	color: #333333;
}

#cp-main .panel {
	background-color: #F9F9F9;
}

#cp-main .pm {
	background-color: #FFFFFF;
}

#cp-main span.corners-top, #cp-menu span.corners-top {
	background-image: url("./styles/prosilver/theme/images/corners_left2.gif");
}

#cp-main span.corners-top span, #cp-menu span.corners-top span {
	background-image: url("./styles/prosilver/theme/images/corners_right2.gif");
}

#cp-main span.corners-bottom, #cp-menu span.corners-bottom {
	background-image: url("./styles/prosilver/theme/images/corners_left2.gif");
}

#cp-main span.corners-bottom span, #cp-menu span.corners-bottom span {
	background-image: url("./styles/prosilver/theme/images/corners_right2.gif");
}

/* Topicreview */
#cp-main .panel #topicreview span.corners-top, #cp-menu .panel #topicreview span.corners-top {
	background-image: url("./styles/prosilver/theme/images/corners_left.gif");
}

#cp-main .panel #topicreview span.corners-top span, #cp-menu .panel #topicreview span.corners-top span {
	background-image: url("./styles/prosilver/theme/images/corners_right.gif");
}

#cp-main .panel #topicreview span.corners-bottom, #cp-menu .panel #topicreview span.corners-bottom {
	background-image: url("./styles/prosilver/theme/images/corners_left.gif");
}

#cp-main .panel #topicreview span.corners-bottom span, #cp-menu .panel #topicreview span.corners-bottom span {
	background-image: url("./styles/prosilver/theme/images/corners_right.gif");
}

/* Friends list */
.cp-mini {
	background-color: #eef5f9;
}

dl.mini dt {
	color: #425067;
}

/* PM Styles
----------------------------------------*/
/* PM Message history */
.current {
	color: #000000 !important;
}

/* PM panel adjustments */
.pm-panel-header,
#cp-main .pm-message-nav {
	border-bottom-color: #A4B3BF;
}

/* PM marking colours */
.pmlist li.pm_message_reported_colour, .pm_message_reported_colour {
	border-left-color: #BC2A4D;
	border-right-color: #BC2A4D;
}

.pmlist li.pm_marked_colour, .pm_marked_colour {
	border-color: #FF6600;
}

.pmlist li.pm_replied_colour, .pm_replied_colour {
	border-color: #A9B8C2;
}

.pmlist li.pm_friend_colour, .pm_friend_colour {
	border-color: #5D8FBD;
}

.pmlist li.pm_foe_colour, .pm_foe_colour {
	border-color: #000000;
}

/* Avatar gallery */
#gallery label {
	background-color: #FFFFFF;
	border-color: #CCC;
}

#gallery label:hover {
	background-color: #EEE;
}

/*  	
--------------------------------------------------------------
Colours and backgrounds for forms.css
-------------------------------------------------------------- */

/* General form styles
----------------------------------------*/
select {
	border-color: #666666;
	background-color: #FAFAFA;
	color: #000;
}

label {
	color: #425067;
}

option.disabled-option {
	color: graytext;
}

/* Definition list layout for forms
---------------------------------------- */
dd label {
	color: #333;
}

/* Hover effects */
fieldset dl:hover dt label {
	color: #000000;
}

fieldset.fields2 dl:hover dt label {
	color: inherit;
}

/* Quick-login on index page */
fieldset.quick-login input.inputbox {
	background-color: #F2F3F3;
}

/* Posting page styles
----------------------------------------*/

#message-box textarea {
	color: #333333;
}

/* Input field styles
---------------------------------------- */
.inputbox {
	background-color: #FFFFFF; 
	border-color: #B4BAC0;
	color: #333333;
}

.inputbox:hover {
	border-color: #11A3EA;
}

.inputbox:focus {
	border-color: #11A3EA;
	color: #0F4987;
}

/* Form button styles
---------------------------------------- */

a.button1, input.button1, input.button3, a.button2, input.button2 {
	color: #000;
	background-color: #FAFAFA;
	background-image: url("./styles/prosilver/theme/images/bg_button.gif");
}

a.button1, input.button1 {
	border-color: #666666;
}

input.button3 {
	background-image: none;
}

/* Alternative button */
a.button2, input.button2, input.button3 {
	border-color: #666666;
}

/* <a> button in the style of the form buttons */
a.button1, a.button1:link, a.button1:visited, a.button1:active, a.button2, a.button2:link, a.button2:visited, a.button2:active {
	color: #000000;
}

/* Hover states */
a.button1:hover, input.button1:hover, a.button2:hover, input.button2:hover, input.button3:hover {
	border-color: #BC2A4D;
	color: #BC2A4D;
}

input.search {
	background-image: url("./styles/prosilver/theme/images/icon_textbox_search.gif");
}

input.disabled {
	color: #666666;
}
.reputation.positive a, .reputation.negative a {
	color: white;
	text-decoration: none;
	cursor: pointer;
}
.reputation.positive {
	background: #6F8F52;
	color: white;
	text-align: center;
}

.reputation.negative {
	background: #B82929;
	color: white;
	text-align: center;
}

.reputation.zero {
	background: #FFF;
	color: #6E6E6E;
	text-align: center;
}

.reputation.span {
	color: #FFF;
}

.reputation {
	font-weight: bold;
	padding: 3px 8px;
	display: inline-block;
	-moz-border-radius: 3px;
	-webkit-border-radius: 3px;
	border-radius: 3px;
	font-size: 10px;
	/*height: 14px;*/
	/*padding: 2px 10px !important;*/
}

.positive {
	color: #6F8F52;
}

.negative {
	color: #B82929;
}

ul.profile-icons li.rate-good-icon	{ width: 20px; height: 20px; }
ul.profile-icons li.rate-bad-icon	{ width: 20px; height: 20px; }

ul.profile-icons li.rate-good-icon a	{ background-image: url("./styles/prosilver/imageset/icon_rate_good.gif"); }
ul.profile-icons li.rate-bad-icon a	{ background-image: url("./styles/prosilver/imageset/icon_rate_bad.gif"); }

ul.profile-icons li.rate-good-icon a:hover,
ul.profile-icons li.rate-bad-icon a:hover {background-position: center bottom}

#repo-popup {
    width:  300px;
    font-size: 80%;
    position: absolute;
    z-index: 500;
    display: none;
    background: white;
    box-shadow: 2px 2px 4px #555555;
    border-radius: 11px;
    padding: 5px;
}
#repo-popup .panel {margin-bottom: 0px}
#repo-popup .error {text-align: center; font-size:140%}

.vote_status_positive, .vote_status_negative {text-align: center; font-size: 120%;}
.vote_status_positive {background: #d4f0cc}
.vote_status_negative {background: #ffe6e6}

.rs_more_details {background: #FFFFFF; padding: 3px;font-size:110%;border-radius: 0 0 3px 3px ;border:1px solid #0F9BE1;margin: -5px auto 5px;width:100px;text-align: center;}
/**
*
* @package mChat ProSilver Style
* @version 1.3.5 2010-12-02
* @copyright (c) RMcGirr83 ( http://www.rmcgirr83.org/ )
* @copyright (c) By Shapoval Andrey Vladimirovich (AllCity) ~ http://allcity.net.ru/
* @license http://opensource.org/licenses/gpl-license.php GNU Public License
*
**/

/* mChat
------------ */
div.mChatBG1 {
	background-color: #FBFBFB;
	padding: 5px 5px 5px 5px;
	border-bottom: 1px dotted #DDDDDD;
	overflow: hidden;
}

div.mChatBG2 {
	background-color: #FFFFFF;
	padding: 5px 5px 5px 5px;
	border-bottom: 1px dotted #DDDDDD;
	overflow: hidden;
}

div.mChatRowLimit {
	overflow: auto;
	width: 100%;
}

div.mChatRowLimitCustom {
	overflow: auto;
	width: 100%;
}

div.mChatPanel {
	text-align: center;
	padding: 3px;
	clear: both;
}

input.mChatText {
	cursor: text;
	width: 50%;
	background-color: #FFFFFF;
	border: 1px solid #B4BAC0;
	color: #333333;
	padding: 5px 5px 3px 5px;
	margin: 3px 0px 3px 5px;
}

input.mChatText:hover {
	border-color: #11A3EA;
}

div.mChatHover:hover {
	background-color: #F6F4D0;
}

div.mChatBodyFix {
	width: 100% !important;
	background-color: #E9F0F5 !important;
}

div.mChatStatic {
	padding-left: 5px;
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	text-align: left;
	font-size: 1.1em;
}

div.mChatStats {
	padding-left: 5px;
	margin-top: 2px;
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	text-align: left;
	min-height: 1.3em;
	font-size: 1.1em;
	height: auto !important;
}

div.mChatRefresh {
	padding-left: 5px;
	font-family: "Lucida Grande", Verdana, Helvetica, Arial, sans-serif;
	text-align: left;
	font-size: 1.1em;
	margin-top: 1.5em;
	clear: both;
}

input.mChatColor {
	width: 100%;
	text-align: center;
	background-color: #DEE3E7;
	border-style: none;	
}

div.mChatMessage {
	padding: 3px;
	font-size: 1.1em;
	width: 98%;

}

a.mChatScriptLink {
	text-decoration:none;
}

img.mChatImage{
	vertical-align: middle;
}

img.mChatImageLoad {
	vertical-align: middle;
	cursor: wait;
	display: none;
}

img.mChatImageOk {
	vertical-align: middle;
	cursor: help;
}

img.mChatImageHideImg {
	vertical-align: middle; 
	cursor: help; 
	display: none;
}

div.mChatSound {
	position: absolute;
	left: -1000px;
	top: -1000px;
}

.mchat_alert {
	color: #7E2217;
	padding: 10px;
}

#mChatUserList {
	display: none;
	float: left;
}

.mChatAvatars {
	float: left;
	padding-right: 5px;
}

div.avatarMessage {
	margin-left: 50px;
	width: 90%;
	margin-right: 5px;
}