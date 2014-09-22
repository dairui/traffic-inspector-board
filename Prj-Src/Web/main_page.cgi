i pg_header.inc
t <a class="cur">基本信息</a>
t <a href="/system.cgi">系统管理</a>
t <a href="/aplication_set.cgi">应用设置</a>
t <a href="/network.cgi">IP设置</a>
t </td><td valign="top">
t <style>
t #m_table{margin:20px 0px 0px 20px;}
t #m_table tr td{padding:5px 30px 5px 30px;}
t #m_table tr td{background:#eee;}
t #m_table tr td.desc{background:#ccc;}
t </style>
t <form name="cgi" action="main_page.cgi" >
t <table id="m_table" border="0" cellspacing="1">
c d 1 <tr><td nowrap class="desc">设备信息</td><td nowrap>%s</td></tr>
c d 2 <tr><td nowrap class="desc">设备地点</td><td nowrap>%s</td></tr>
c d a <tr><td nowrap class="desc">设备型号</td><td nowrap>%s</td></tr>
c d 3 <tr><td nowrap class="desc">软件版本号</td><td nowrap>%s</td></tr>
c d 4 <tr><td nowrap class="desc">MAC地址</td><td nowrap>%s-%s-%s-%s-%s-%s</td></tr>
c d 5 <tr><td nowrap class="desc">本机IP</td><td nowrap>%d.%d.%d.%d</td></tr>
c d 6 <tr><td nowrap class="desc">服务器IP</td><td nowrap>%d.%d.%d.%d</td></tr>
c d 7 <tr><td nowrap class="desc">服务器端口</td><td nowrap>%s</td></tr>
c d 8 <tr><td nowrap class="desc">交流电电压比</td><td nowrap>%.1f</td></tr>
c d 9 <tr><td nowrap class="desc">交流电压实时监测(F5刷新)</td><td nowrap>%d V</td></tr>
c d b <tr><td nowrap class="desc">是否学习</td><td nowrap>%s</td></tr>
c d c <tr><td nowrap class="desc">学习的有效通道(顺序:绿黄红)</td><td nowrap>%s</td></tr>
c d e <tr><td nowrap class="desc">学习的通道电流(顺序:绿黄红)</td><td nowrap>%d %d %d %d %d %d %d %d %d %d %d %d</td></tr>
c d f <tr><td nowrap class="desc">周期时长</td><td nowrap>%d s</td></tr>
c d d <tr><td nowrap class="desc">当前阶段表</td><td nowrap>%s%d%s%d%d%d%d%d%d%d%d 绿%ds</td></tr>
t <tr><td nowrap>&nbsp;</td><td nowrap class="btns">
t <input type="submit" value="刷新" id="confirm"/></td></tr></table></form>
i pg_footer.inc
. End of script must be closed with period.