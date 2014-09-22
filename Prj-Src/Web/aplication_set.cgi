i pg_header.inc
t <a href="/main_page.cgi">基本信息</a>
t <a href="/system.cgi">系统管理</a>
t <a class="cur">应用设置</a>
t <a href="/network.cgi">IP设置</a>
t </td><td valign="top">
t <style>
t #m_table{margin:20px 0px 0px 20px;}
t #m_table tr td{padding:5px 30px 5px 30px;background:#fff;}
t #m_table tr td.ti{font-weight:bold;padding:20px 30px 5px 5px;}
t #m_table tr td.val{background:#eee;}
t #m_table tr td.desc{background:#ccc;}
t #m_table tr td.fre{background:#66ff33;}
t #m_table tr td.btns{padding:20px 30px 20px 0px;}
t #m_table tr td.btns input{width:80px;height:30px;}
t </style>
t <form name="cgi" action="aplication_set.cgi" method="get" onsubmit="return frmsubmit();">
t <table id="m_table" border="0" cellspacing="1">
t <tr><td nowrap class="desc">交流电校准电压</td>
t <td nowrap class="val">
c t a <input type="text" name="turn_num" value="%.0f" size="9" maxlength=9>V</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="stand_vol" >更改</td></tr>
#t <tr><td nowrap class="desc">交流电电压比</td><td nowrap class="val">
#c t f <input type="text" name="turn_num_lxb" value="%.0f" size="9" maxlength=9></td>
#t <td nowrap class="val"><input type="checkbox" name="appbox" value ="stand_tun" >更改</td></tr>
t <tr><td nowrap class="desc">蓄电池异常报警值</td><td nowrap class="val">
c t b <input type="text" name="bat_min" value="%d" size="9" maxlength="9">分钟</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="bat_alarm" >更改</td></tr>
t <tr><td nowrap class="desc">信号灯异常报警阀值</td><td nowrap class="val">
c t c <input type="text" name="cur_min" value="%d" size="9" maxlength="9">%%</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="lamp_alarm" >更改</td></tr>
t <tr><td nowrap class="desc">信号灯电流上报周期</br>交流电或绿冲上报周期</td>
t <td nowrap class="val">
c t d <input type="text" name="send_time" value="%d" size="9" maxlength="9">分钟</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="lamp_current" >更改</td></tr>
t <tr><td nowrap>&nbsp;</td><td nowrap class="btns">
t <input type="submit" value="确定" id="confirm"/>
t <input type="reset" value="重置"/></td></tr></table></form>
t <script>function frmsubmit(){return confirm('确定更改应用设置?');}</script>
i pg_footer.inc
. End of script must be closed with period.