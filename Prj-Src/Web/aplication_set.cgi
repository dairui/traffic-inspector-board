i pg_header.inc
t <a href="/main_page.cgi">������Ϣ</a>
t <a href="/system.cgi">ϵͳ����</a>
t <a class="cur">Ӧ������</a>
t <a href="/network.cgi">IP����</a>
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
t <tr><td nowrap class="desc">������У׼��ѹ</td>
t <td nowrap class="val">
c t a <input type="text" name="turn_num" value="%.0f" size="9" maxlength=9>V</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="stand_vol" >����</td></tr>
#t <tr><td nowrap class="desc">�������ѹ��</td><td nowrap class="val">
#c t f <input type="text" name="turn_num_lxb" value="%.0f" size="9" maxlength=9></td>
#t <td nowrap class="val"><input type="checkbox" name="appbox" value ="stand_tun" >����</td></tr>
t <tr><td nowrap class="desc">�����쳣����ֵ</td><td nowrap class="val">
c t b <input type="text" name="bat_min" value="%d" size="9" maxlength="9">����</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="bat_alarm" >����</td></tr>
t <tr><td nowrap class="desc">�źŵ��쳣������ֵ</td><td nowrap class="val">
c t c <input type="text" name="cur_min" value="%d" size="9" maxlength="9">%%</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="lamp_alarm" >����</td></tr>
t <tr><td nowrap class="desc">�źŵƵ����ϱ�����</br>��������̳��ϱ�����</td>
t <td nowrap class="val">
c t d <input type="text" name="send_time" value="%d" size="9" maxlength="9">����</td>
t <td nowrap class="val"><input type="checkbox" name="appbox" value ="lamp_current" >����</td></tr>
t <tr><td nowrap>&nbsp;</td><td nowrap class="btns">
t <input type="submit" value="ȷ��" id="confirm"/>
t <input type="reset" value="����"/></td></tr></table></form>
t <script>function frmsubmit(){return confirm('ȷ������Ӧ������?');}</script>
i pg_footer.inc
. End of script must be closed with period.