i pg_header.inc
t <a href="/main_page.cgi">基本信息</a>
t <a class="cur">系统管理</a>
t <a href="/aplication_set.cgi">应用设置</a>
t <a href="/network.cgi">IP设置</a>
t </td><td valign="top">
t <style>
t #m_table{margin:0px 0px 0px 20px;}
t #m_table tr td{padding:5px 30px 5px 30px;background:#fff;}
t #m_table tr td.ti{font-weight:bold;padding:20px 30px 5px 5px;}
t #m_table tr td.zs{font-weight:bold;padding:20px 250px 5px 5px;}
t #m_table tr td.val{background:#eee;}
t #m_table tr td.desc{background:#ccc;}
t #m_table tr td.btns{padding:20px 30px 20px 0px;}
t #m_table tr td.btns input{width:80px;height:30px;}
t </style>
t <form name="cgi" action="system.cgi" method="get" onsubmit="return frmsubmit();">
t <table id="m_table" border="0" cellspacing="1">
t <tr><td nowrap class="ti">基本设置</td><td nowrap>&nbsp;</td></tr>
t <tr><td nowrap class="desc">恢复出厂设置</td><td nowrap class="val">
t <input type="radio" name="radio1"  value="yes" id="rad1A" >
t <label for="rad1A">是</label>
t <input type="radio" name="radio1" checked="checked" value="no" id="rad1B" >
t <label for="rad1B">否</label>
t </td></tr>
t <tr><td nowrap class="desc">重新学习信号机</td><td nowrap class="val">
t <input type="radio" name="radio2"  value="yes" id="rad2A" >
t <label for="rad2A" class="slab">是</label>
t <input type="radio" name="radio2" checked="checked" value="no" id="rad2B" >
t <label for="rad2B" class="slab">否</label>
t </td></tr>
t <tr><td nowrap class="desc">重启设备</td><td nowrap class="val">
t <input type="radio" name="radio3" value="yes" id="rad3A" >
t <label for="rad3A" class="slab">是</label>
t <input type="radio" name="radio3" checked="checked" value="no" id="rad3B" >
t <label for="rad3B" class="slab">否</label>
t </td></tr>
t <tr><td nowrap class="ti">通道组合设置(见**注)</td><td nowrap>&nbsp;</td></tr>
t <tr><td nowrap class="desc">组合1</td><td nowrap class="val">
c v a <input type="text" name="combine_pas1" value="%d" size="9" maxlength=9>
t <input type="checkbox" name="combbox" value ="comb_pas1" ></td></tr>
t <tr><td nowrap class="desc">组合2</td><td nowrap class="val">
c v b <input type="text" name="combine_pas2" value="%d" size="9" maxlength=9>
t <input type="checkbox" name="combbox" value ="comb_pas2" ></td></tr>
t <tr><td nowrap class="desc">组合3</td><td nowrap class="val">
c v c <input type="text" name="combine_pas3" value="%d" size="9" maxlength=9>
t <input type="checkbox" name="combbox" value ="comb_pas3" ></td></tr>
t <tr><td nowrap class="desc">组合4</td><td nowrap class="val">
c v d <input type="text" name="combine_pas4" value="%d" size="9" maxlength=9>
t <input type="checkbox" name="combbox" value ="comb_pas4" ></td></tr>
#t <tr><td nowrap class="desc">组合5</td><td nowrap class="val">
#c v e <input type="text" name="combine_pas5" value="%d" size="9" maxlength=9>
#t <input type="checkbox" name="combbox" value ="comb_pas5" ></td></tr>
#t <tr><td nowrap class="desc">通道组合6</td><td nowrap class="val">
#c v f <input type="text" name="combine_pas6" value="%d" size="9" maxlength=9>
#t <input type="checkbox" name="combbox" value ="comb_pas6" ></td></tr>
t <tr><td nowrap class="ti">定时重启设置</td><td nowrap>&nbsp;</td></tr>
c g a <tr><td nowrap class="desc">当前定时重启状态</td><td nowrap class="val">%s</td></tr>
t <tr><td nowrap class="desc">定时重启</td><td nowrap class="val">
t <input type="radio" name="radio4"  value="yes" id="rad4A" >
t <label for="rad4A" class="slab">启用</label>
t <input type="radio" name="radio4" checked="checked" value="no" id="rad4B" >
t <label for="rad4B" class="slab">禁用</label>
t </td></tr>
t <tr><td nowrap class="desc">定时重启</td><td nowrap class="val">
t <select name="restart_hour">
c b 1 <option>00</option><option>01</option><option>02</option><option>03</option><option>04</option>
c b 1 <option>05</option><option>06</option><option>07</option><option>08</option><option>09</option>
c b 1 <option>10</option><option>11</option><option>12</option><option>13</option><option>14</option>
c b 1 <option>15</option><option>16</option><option>17</option><option>18</option><option>19</option>
c b 1 <option>20</option><option>21</option><option>22</option><option>23</option></select>
t 时<select name="restart_minute">
c b 2 <option>00</option><option>01</option><option>02</option><option>03</option><option>04</option>
c b 2 <option>05</option><option>06</option><option>07</option><option>08</option><option>09</option>
c b 2 <option>10</option><option>11</option><option>12</option><option>13</option><option>14</option>
c b 2 <option>15</option><option>16</option><option>17</option><option>18</option><option>19</option>
c b 2 <option>20</option><option>21</option><option>22</option><option>23</option><option>24</option>
c b 2 <option>25</option><option>26</option><option>27</option><option>28</option><option>29</option>
c b 2 <option>30</option><option>31</option><option>32</option><option>33</option><option>34</option>
c b 2 <option>35</option><option>36</option><option>37</option><option>38</option><option>39</option>
c b 2 <option>40</option><option>41</option><option>42</option><option>43</option><option>44</option>
c b 2 <option>45</option><option>46</option><option>47</option><option>48</option><option>49</option>
c b 2 <option>50</option><option>51</option><option>52</option><option>53</option><option>54</option>
c b 2 <option>55</option><option>56</option><option>57</option><option>58</option><option>59</option></select>
t 分</td></tr>
t <tr><td nowrap>&nbsp;</td><td nowrap class="btns">
t <input type="submit" value="确定" id="confirm"/>
t <input type="reset" value="重置"/>
t </td></tr>
t </table>
t <b>&nbsp注：</b><br />
t <b>&nbsp;&nbsp;&nbsp通道1=1;</b><br />
t <b>&nbsp;&nbsp;&nbsp通道2=2;</b><br />
t <b>&nbsp;&nbsp;&nbsp通道3=4;</b><br />
t <b>&nbsp;&nbsp;&nbsp通道4=8;</b><br />
t <b>&nbsp;&nbsp;&nbsp如通道1与通道4组合，则组合的值为:8+1=9;</b><br />
t <b>&nbsp;&nbsp;&nbsp最大支持4个组合</b><br />
t </form>
t <script>function frmsubmit(){return confirm('确定执行系统管理?');}</script>
i pg_footer.inc
. End of script must be closed with period.