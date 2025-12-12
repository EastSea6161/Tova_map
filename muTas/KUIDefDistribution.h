#pragma once
namespace UIDistribution
{
const CString
    UIXAML_D_BACKIMAGE
        (
        _T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#8e8269' Offset='0'/>")
        _T("      <GradientStop Color='#cbc1b4' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")
        _T("</Border>")
        );

const CString
	UIXAML_D_CALCULATE_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> 거리/시간 계산</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_CALCULATE_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> 거리/시간 계산</TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_CALCULATE_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> 거리/시간 계산</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_INPUT_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Input(Xi, Tj)</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
	KR_UIXAML_D_INPUT_FALSE
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>분석 년도 유출/유입량</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);   

const CString
    UIXAML_D_INPUT_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Input(Xi, Tj)</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
	KR_UIXAML_D_INPUT_DEFAULT
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>분석 년도 유출/유입량</TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);


const CString
    UIXAML_D_INPUT_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Input(Xi, Tj)</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
	KR_UIXAML_D_INPUT_SELECTED 
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>분석 년도 유출/유입량</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_BASEOD_FALSE
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
    _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Base OD</TextBlock>")    
    _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Distribution</TextBlock>")   
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_BASEOD_FALSE
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>기준 년도 O/D</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_BASEOD_DEFAULT
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
    _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
    _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Base OD</TextBlock>")    
    _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333' HorizontalAlignment='Center'> Distribution</TextBlock>")  
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_BASEOD_DEFAULT
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>기준 년도 O/D</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_BASEOD_SELECTED 
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#378BBA' Offset='0'/>")
    _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
    _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Base OD</TextBlock>")    
    _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366' HorizontalAlignment='Center'> Distribution</TextBlock>")  
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_BASEOD_SELECTED 
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")     
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>기준 년도 O/D</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_VARIABLE_FALSE
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
    _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Impedance</TextBlock>")    
    _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Variable</TextBlock>")   
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_VARIABLE_FALSE
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>중력모형 통행저항함수</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_VARIABLE_DEFAULT
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
    _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
    _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Impedance</TextBlock>")    
    _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333' HorizontalAlignment='Center'> Variable</TextBlock>")  
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_VARIABLE_DEFAULT
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>중력모형 통행저항함수</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_VARIABLE_SELECTED 
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#378BBA' Offset='0'/>")
    _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
    _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Impedance</TextBlock>")    
    _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366' HorizontalAlignment='Center'> Variable</TextBlock>")  
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_VARIABLE_SELECTED 
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")     
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>중력모형 통행저항함수</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_STOP_FALSE
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
    _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Stopping Criteria</TextBlock>")    
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_STOP_FALSE
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>반복수행 종료조건</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_STOP_DEFAULT
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
    _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Stopping Criteria</TextBlock>")     
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_STOP_DEFAULT
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>반복수행 종료조건</TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_STOP_SELECTED 
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#378BBA' Offset='0'/>")
    _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Stopping Criteria</TextBlock>")    
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_STOP_SELECTED 
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>반복수행 종료조건</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_MODEL_DEFAULT 
        (
        _T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90915'/></TextBlock>")    
        _T("    <Border Height='1' Background='#ddc09e' />")
        _T("    <Border Height='1' Background='#ffffff' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Growth Factor(Fratar)</TextBlock>")    
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Gravity</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
	KR_UIXAML_D_MODEL_DEFAULT 
	(
	_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 	_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90915'/></TextBlock>")    
// 	_T("    <Border Height='1' Background='#ddc09e' />")
// 	_T("    <Border Height='1' Background='#ffffff' />")
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>적용 모형 설정</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_MODEL_SELECTED 
        (
        _T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90915'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#7c96b5' />")
        _T("    <Border Height='1' Background='#9acae4' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Growth Factor(Fratar)</TextBlock>")    
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Gravity</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
    KR_UIXAML_D_MODEL_SELECTED 
        (
        _T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
//         _T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90915'/> </TextBlock>")    
//         _T("    <Border Height='1' Background='#7c96b5' />")
//         _T("    <Border Height='1' Background='#9acae4' />")
        _T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>적용 모형 설정</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );

const CString
    UIXAML_D_OUTPUT_FALSE
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/>  </TextBlock>")    
        _T("    <Border Height='1' Background='#a0a5a8' />")
        _T("    <Border Height='1' Background='#cdd1d4' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
    KR_UIXAML_D_OUTPUT_FALSE
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
//         _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/>  </TextBlock>")    
//         _T("    <Border Height='1' Background='#a0a5a8' />")
//         _T("    <Border Height='1' Background='#cdd1d4' />")
        _T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>출력 설정</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );

const CString
    UIXAML_D_OUTPUT_DEFAULT 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#ddc09e' />")
        _T("    <Border Height='1' Background='#ffffff' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
    KR_UIXAML_D_OUTPUT_DEFAULT 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
//         _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/> </TextBlock>")    
//         _T("    <Border Height='1' Background='#ddc09e' />")
//         _T("    <Border Height='1' Background='#ffffff' />")
        _T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>출력 설정</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );

const CString
        UIXAML_D_OUTPUT_SELECTED 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#7c96b5' />")
        _T("    <Border Height='1' Background='#9acae4' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );
const CString
        KR_UIXAML_D_OUTPUT_SELECTED 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
//         _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/> </TextBlock>")    
//         _T("    <Border Height='1' Background='#7c96b5' />")
//         _T("    <Border Height='1' Background='#9acae4' />")
        _T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>출력 설정</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );


const CString
    UIXAML_D_VTABLE_FALSE
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
    _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    //_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> View Table</TextBlock>")    
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Result View</TextBlock>")
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_VTABLE_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>결과보기</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_VTABLE_DEFAULT
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
    _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    //_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> View Table</TextBlock>")     
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Result View</TextBlock>")
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_VTABLE_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>결과보기</TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_VTABLE_SELECTED 
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#378BBA' Offset='0'/>")
    _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    //_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> View Table</TextBlock>")    
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Result View</TextBlock>")
    _T("  </StackPanel>")
    _T("</Border>")
    );
const CString
	KR_UIXAML_D_VTABLE_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>결과보기</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_DGRAPH_FALSE
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
    _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Draw Graph</TextBlock>")    
    _T("  </StackPanel>")
    _T("</Border>")
    );

const CString
    UIXAML_D_DGRAPH_DEFAULT
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
    _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Draw Graph</TextBlock>")     
    _T("  </StackPanel>")
    _T("</Border>")
    );

const CString
    UIXAML_D_DGRAPH_SELECTED 
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#378BBA' Offset='0'/>")
    _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Draw Graph</TextBlock>")    
    _T("  </StackPanel>")
    _T("</Border>")
    );
}